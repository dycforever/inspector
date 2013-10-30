#!/usr/bin/python3
'''
Created on 2 Sep 2013

@author: dyc
'''
import sys
import os
import subprocess
from gi.repository import Gtk, Pango

print("python path: %s" % (sys.path))
import mesg_pb2

class PosTransfer:
    def __init__(self, textbuffer):
        self.textbuffer = textbuffer

        # for hexdump: leading address and two SPACE
        # for xxd: leading address and a comma
        self.ADDRLEN = 9

        # for hexdump: length of tailing |... BINARY CONTENTS ...|\n
        # 2 + 16 + 1 = 19
        # for xxd: length of tailing BINARY CONTENTS \n
        # 16 + 1 = 17
        self.CONTLEN = 17

        self.BYTES_PER_LINE = 16
        self.LINELEN = (self.ADDRLEN + self.CONTLEN + self.BYTES_PER_LINE * 3 + 1 + 1)
        self.HALF = 8

    def in_first_half(self, mod):
        return 1 if mod >= self.HALF else 0

    def addr_to_off(self, addr):
        line_number = addr//self.BYTES_PER_LINE
        line_offset = self.ADDRLEN + addr%self.BYTES_PER_LINE*3 + self.in_first_half(addr%self.BYTES_PER_LINE)
        print("add[%d] at line[%d] offset[%d] ---> [%d]" % (addr, line_number, addr%self.BYTES_PER_LINE, line_number * self.LINELEN + line_offset))
        return (line_number * self.LINELEN + line_offset)

    def lower_bound (self, addr):
#        print("addr %d get lower bound %d" % (addr, (addr//self.BYTES_PER_LINE) * self.BYTES_PER_LINE) )
        return (addr//self.BYTES_PER_LINE) * self.BYTES_PER_LINE

    def upper_bound (self, addr):
#        print("addr %d get upper bound %d" % (addr, self.lower_bound(addr) + self.BYTES_PER_LINE - 1))
        return self.lower_bound(addr) + self.BYTES_PER_LINE - 1

    def in_same_line (self, addr_start, addr_end):
        return True if (addr_end//self.BYTES_PER_LINE == addr_start//self.BYTES_PER_LINE) else False

    # the parameter is a [) address range
    # return : a list of [addr, addr]
    def split_range(self, range_start, range_end):
        range_end -= 1
        addr_range = []
        while range_start <= range_end and self.in_same_line(range_start, range_end) == False :
#            print("split line (%d %d)" %(range_start, range_end) )
            addr_range.append( (range_start, self.upper_bound(range_start)) )
            range_start = self.upper_bound(range_start) + 1
        addr_range.append( (range_start, range_end) )
#        for a in addr_range:
#            print("addr: %s" %(a.__str__()) )
        return addr_range

    # the parameter is a [) address range
    def trans(self, range_start, range_end):
        addr_range = self.split_range(range_start, range_end)
        ret = []
        for r in addr_range :
            iter1 = self.textbuffer.get_start_iter().copy()
            iter2 = self.textbuffer.get_start_iter().copy()
            iter1.forward_chars( self.addr_to_off(r[0]))
            iter2.forward_chars( self.addr_to_off(r[1]) + 2)
            ret.append( (iter1, iter2) )
        return ret






class InspectorMainWin(Gtk.Window):
    def loadmesg(self):
        self.mesg = mesg_pb2.HeaderMesg()
        f = open("mid.out", "rb")
        self.mesg.ParseFromString(f.read())
        f.close()
    def filltext(self, text):
        return text.replace("*\n", '*' + ' ' * (self.transfer.LINELEN - 2) + '\n')

    # add a whitespace in the middle of 16 bytes
    def decorateText(self, text):
        new_text=''
        for line in text.split("\n"):
            new_text += line[0:33] + ' ' + line[33:] + '\n'
        return new_text

    def getStrTab(self):
        for sh in self.mesg.secHeaders:
            print("strtab %x\n" % sh.type)
            if sh.type == 3:
                print("found strtab")
#        self.strtab =

    def deliminateSections(self):
        for secheader in self.mesg.secHeaders:
            print("deliminate header offset: %s \n"% hex(secheader.offset))
            pos = self.transfer.addr_to_off(secheader.offset)
            textiter = self.textbuffer.get_start_iter().copy()
            textiter.forward_chars(pos-1)
            textiter2 = textiter.copy()
            textiter2.forward_char()
            self.textbuffer.apply_tag(self.secDelimitTag, textiter, textiter2)


    def memberInit(self):
        self.textview = Gtk.TextView()
        self.textbuffer = self.textview.get_buffer()
        self.textview2 = Gtk.TextView()
        self.textview2.set_wrap_mode(Gtk.WrapMode.CHAR)
        self.textbuffer2 = self.textview2.get_buffer()

        self.elfhead_tag = self.textbuffer.create_tag("elf head", foreground = "red")
        self.sechead_tag = self.textbuffer.create_tag("section head", foreground = "blue")
        self.sechead_tag_ = self.textbuffer.create_tag("sec head head", foreground = "blue", background = "yellow")
        self.exec_tag = self.textbuffer.create_tag("exec", background = "blue")
        self.proghead_tag = self.textbuffer.create_tag("program head", foreground = "green")
        self.proghead_tag_ = self.textbuffer.create_tag("prog head head", foreground = "green", background = "yellow")
        self.secDelimitTag = self.textbuffer.create_tag("section delimiter", background = "blue")

        self.transfer = PosTransfer(self.textbuffer)


    def deployWidget(self):
        self.set_default_geometry(400, 400);
        self.set_default_size(1000, 400)
        self.set_position(Gtk.WindowPosition.CENTER)

        self.grid = Gtk.Grid()
        self.add(self.grid)
        scrolledwindow = Gtk.ScrolledWindow()
        scrolledwindow.set_hexpand(True)
        scrolledwindow.set_vexpand(True)
        self.grid.attach(scrolledwindow, left=0, top=0, width=1, height=1)
        scrolledwindow.add(self.textview)

        label = Gtk.Label()
        label.set_text(" ")
        self.grid.attach_next_to(label, scrolledwindow, Gtk.PositionType.RIGHT, width=1, height=1)

        scrolledwindow2 = Gtk.ScrolledWindow()
        scrolledwindow2.set_hexpand(True)
        self.grid.attach_next_to(scrolledwindow2, label, Gtk.PositionType.RIGHT, width=1, height=1)
        scrolledwindow2.add(self.textview2)

        button = Gtk.Button(label="wrap mode")
#        self.grid.attach_next_to(button, scrolledwindow2, Gtk.PositionType.BOTTOM, width=1, height=1)


    def __init__(self, filename):
        Gtk.Window.__init__(self, title="Inspector")
        self.loadmesg()
        self.getStrTab();

        self.memberInit();

#        cmd = "hexdump -C "+ filename + " | cut -c 11- | cut -d' ' -f1-18"
#        cmd = "hexdump -C "+ filename
        cmd = "xxd -g1 "+ filename
        fileContent = subprocess.check_output(cmd, shell=True)
        self.textbuffer.set_text(self.decorateText(fileContent.decode("ascii")))

        cmd = "readelf -e "+ filename
        fileContent = subprocess.check_output(cmd, shell=True)
        self.textbuffer2.set_text(fileContent.decode("ascii"))

        self.deployWidget()

        iterlist = self.transfer.trans(0, self.mesg.elfHeader.ehsize)
        for it in iterlist:
#            print("apply tag %s" %(it.__str__()))
            self.textbuffer.apply_tag(self.elfhead_tag, it[0], it[1])

        self.deliminateSections()
#        self.showExec(self.mesg.progHeaders, self.exec_tag)
        self.apply_headers(self.sechead_tag, self.mesg.secHeaders, self.sechead_tag_)
        self.apply_headers(self.proghead_tag, self.mesg.progHeaders, self.proghead_tag_)

    def showExec (self, headers, tag):
        for header in headers:
            if (header.flags & 1):
                begin = header.offset
                end = begin + header.filesz
                iterlist = self.transfer.trans(begin, end)
                for it in iterlist:
                    self.textbuffer.apply_tag(tag, it[0], it[1])

    def apply_headers(self, firsttag, headers, tag_):
        for header in headers:
#            print("deal header: %s in apply" % header.__str__())
            iterlist = self.transfer.trans(header.begin, header.end)
            first = True
            for it in iterlist:
                if first == True:
                    it_head = self.textbuffer.get_start_iter().copy()
                    it_head.forward_chars(it[0].get_offset())
                    it_head2 = it_head.copy()
                    it_head2.forward_chars(2)
                    self.textbuffer.apply_tag(tag_, it_head, it_head2)
                    it[0].forward_chars(3)
                    first = False
                self.textbuffer.apply_tag(firsttag, it[0], it[1])
#            break


if __name__ == '__main__':
    if (len(sys.argv)<2):
        filename = "a.out"
    else:
        filename = sys.argv[1]
    win = InspectorMainWin(filename)
    win.connect("delete-event", Gtk.main_quit)
    win.show_all()
    Gtk.main()

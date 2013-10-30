
#include <libelf.h>
#include <gelf.h>
#include <stdio.h>

#include "common.h"
#include "Header.h"

#include "messages/mesg.pb.h"

using namespace dyc;
using namespace std;

int main() {

	fstream input("message.out", ios::in | ios::binary);
	int count = 0;
	while (1)
	{
		count++;
		mesg::HeaderMesg_ProgHeaderMesg mes;
		if (!mes.ParseFromIstream(&input)) {
			cerr << "Failed to parse " << count << "." << endl;
			return -1;
		}
	}
	cout << "success" << endl;
	return 0;

}

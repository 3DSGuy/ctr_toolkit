/**
Copyright 2013 3DSGuy

This file is part of extdata_tool.

make_banner is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

make_banner is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with make_banner.  If not, see <http://www.gnu.org/licenses/>.
**/
#include "lib.h"

void u8_hex_print_be(u8 *array, int len)
{
	for(int i = 0; i < len; i++)
		printf("%02x",array[i]);
}

void u8_hex_print_le(u8 *array, int len)
{
	for(int i = 0; i < len; i++)
		printf("%02x",array[len - i - 1]);
}

int process_dir(u8 *dir, int len)
{
	//TODO
	return 0;
}

int makedir(const char* dir)
{
#ifdef _WIN32
	return _mkdir(dir);
#else
	return mkdir(dir, 0777);
#endif
}
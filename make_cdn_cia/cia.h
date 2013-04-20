/**
Copyright 2013 3DSGuy

This file is part of make_cdn_cia.

make_cdn_cia is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

make_cdn_cia is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with make_cdn_cia.  If not, see <http://www.gnu.org/licenses/>.
**/s
//Sig Types
#define RSA_2048_SHA256 0x04000100
#define RSA_4096_SHA256 0x03000100
#define RSA_2048_SHA1 0x01000100
#define RSA_4096_SHA1 0x00000100
//Errors
#define ERR_UNRECOGNISED_SIG 2

typedef struct
{
	u8 padding_0[0x3c];
	u8 issuer[0x40];
	u8 version;
	u8 ca_crl_version;
	u8 signer_crl_version;
	u8 padding_1;
} __attribute__((__packed__))
TMD_SIG_STRUCT;

typedef struct
{
	u8 content_id[4];
	u8 content_index[2];
	u8 content_type[2];
	u8 content_size[8];
	u8 sha_256_hash[0x20];
} __attribute__((__packed__))
TMD_CONTENT_CHUNK_STRUCT;

typedef struct
{
	TMD_SIG_STRUCT tmd_sig;
	u8 system_version[8];
	u8 title_id[8];
	u8 title_type[4];
	u8 group_id[2];
	u8 reserved[0x3e];
	u8 access_rights[4];
	u8 title_version[2];
	u8 content_count[2];
	u8 boot_content[2];
	u8 padding[2];
	u8 sha_256_hash[0x20];
	u8 content_info_records[0x900];
} __attribute__((__packed__)) 
TMD_STRUCT;

typedef struct
{
	u8 padding_0[0x3c];
	u8 issuer[0x40];
	u8 ECDH[0x3c];
	u8 unknown[3];
} __attribute__((__packed__))
CETK_SIG_STRUCT;

typedef struct
{
	CETK_SIG_STRUCT cetk_sig;
	u8 encrypted_title_key[0x10];
	u8 unknown_0;
	u8 ticket_id[8];
	u8 ticket_consoleID[4];
	u8 title_id[8];
	u8 unknown_1[2];
	u8 title_version[2];
	u8 unused_0[8];
	u8 unused_1;
	u8 common_key_index;
	u8 unknown_2[0x15e];
} __attribute__((__packed__)) 
CETK_STRUCT;

typedef struct
{
	u8 result;

	FILE *tmd;
	u8 title_id[8];
	u16 title_version;
	u32 tmd_size;
	u32 cert_offset[2];
	u32 cert_size[2];
	u16 content_count;
	TMD_CONTENT_CHUNK_STRUCT *content_struct;
	FILE **content;
} __attribute__((__packed__)) 
TMD_CONTEXT;

typedef struct
{
	u8 result;
	
	FILE *cetk;
	u8 title_id[8];
	u16 title_version;
	u32 cetk_size;
	u32 cert_offset[2];
	u32 cert_size[2];
} __attribute__((__packed__)) 
CETK_CONTEXT;

typedef struct
{
	u32 header_size;
	u16 type;
	u16 version;
	u32 cert_size;
	u32 cetk_size;
	u32 tmd_size;
	u32 meta_size;
	u64 content_size;
	u8 magic_0;
	u8 magic_1[0x1fff];
} __attribute__((__packed__)) 
CIA_HEADER;

//Main Function
int generate_cia(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context, FILE *output);

//Processing Functions
CETK_CONTEXT process_cetk(FILE *cetk);
TMD_CONTEXT process_tmd(FILE *tmd);
CIA_HEADER set_cia_header(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context);

//Reading/Calc Functions
u32 get_cetk_size(u32 sig_size);
u32 get_tmd_size(u32 sig_size, u16 content_count);
u32 get_sig_size(u32 offset, FILE *file);
u32 get_cert_size(u32 offset, FILE *file);
u64 get_content_size(TMD_CONTEXT tmd_context);
u64 read_content_size(TMD_CONTENT_CHUNK_STRUCT content_struct);
u32 get_total_cert_size(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context);
u32 get_content_id(TMD_CONTENT_CHUNK_STRUCT content_struct);

//Writing functions
int write_cia_header(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context, FILE *output);
int write_cert_chain(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context, FILE *output);
int write_cetk(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context, FILE *output);
int write_tmd(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context, FILE *output);
int write_content(TMD_CONTEXT tmd_context, CETK_CONTEXT cetk_context, FILE *output);
int write_content_data(FILE *content, u64 content_size, FILE *output);

//Get Struct Functions
CETK_STRUCT get_cetk_struct(u32 sig_size, FILE *cetk);
TMD_STRUCT get_tmd_struct(u32 sig_size, FILE *tmd);
TMD_CONTENT_CHUNK_STRUCT get_tmd_content_struct(u32 sig_size, u8 index, FILE *tmd);

//Printing Functions
void print_content_chunk_info(TMD_CONTENT_CHUNK_STRUCT content_struct);

//Checking Functions
int check_tid(u8 *tid_0, u8 *tid_1);

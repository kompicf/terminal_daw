#include <stdio.h>

struct wawheader{
	char first_thing[4];
	int size;
	char type[4];
	char chunk_marker[4];
	int lenght_of_format_data;
	short type_of_format;
	short num_of_channels;
	int sample_rate;
	int something;
	short another_something;
	short bits_per_sample;
	char beginning_of_data[4];
	int size_of_data_section;
};

void read_waw_header(struct wawheader*header, FILE*file){
	fread(header, 44, 1, file);
}

void print_waw_header(struct wawheader*header){
	printf("first thing: ");
	for(int i=0;i<4;++i){printf("%c",header->first_thing[i]);}
	printf("\n");
	printf("size: %d\n", header->size);

	printf("type: ");
	for(int i=0;i<4;++i){printf("%c",header->type[i]);}
	printf("\n");

	printf("chunk marker: ");
	for(int i=0;i<4;++i){printf("%c",header->chunk_marker[i]);}
	printf("\n");

	printf("lenght of format data: %d\n", header->lenght_of_format_data);
	printf("type of format: %d\n", header->type_of_format);
	printf("num of channels: %d\n", header->num_of_channels);
	printf("sample rate: %d\n", header->sample_rate);
	printf("something: %d\n", header->something);
	printf("another something: %d\n", header->another_something);
	printf("bits per sample: %d\n", header->bits_per_sample);

	printf("beggining of data: ");
	for(int i=0;i<4;++i){printf("%c",header->beginning_of_data[i]);}
	printf("\n");

	printf("size of data section: %d\n", header->size_of_data_section);
}

int main(int argc, char**argv){
	if(argc != 2){
		puts("file name pls");
		return 0;
	}

	FILE*file = fopen(argv[1], "rb");
	struct wawheader header;
	printf("%d\n",sizeof(struct wawheader));
	read_waw_header(&header, file);
	print_waw_header(&header);

	fclose(file);
	return 0;
}



struct fileheader
{
	char riff_label[4]; // (00) = {'R','I','F','F'}
	unsigned riff_size; // (04) = 36 + data_size
	char file_tag[4]; // (08) = {'W','A','V','E'}
	char fmt_label[4]; // (12) = {'f','m','t',' '}
	unsigned fmt_size; // (16) = 16
	unsigned short audio_format; // (20) = 1
	unsigned short channel_count; // (22) = 1 or 2
	unsigned sampling_rate; // (24) = (anything)
	unsigned bytes_per_second; // (28) = (see above)
	unsigned short bytes_per_sample; // (32) = (see above)
	unsigned short bits_per_sample; // (34) = 8 or 16
	char data_label[4]; // (36) = {'d','a','t','a'}
	unsigned data_size; // (40) = # bytes of data
};

void PrintHeader(fileheader* h, char* data, int i)
{
	 	  printf("\n");
	if(i) printf("riff_label:       %c%c%c%c.\n",h->riff_label[0],h->riff_label[1],h->riff_label[2],h->riff_label[3]);
		  printf("riff_size:        %u.\n",h->riff_size);
	if(i) printf("file_tag:         %c%c%c%c.\n",h->file_tag[0],h->file_tag[1],h->file_tag[2],h->file_tag[3]);
	if(i) printf("fmt_label:        %c%c%c%c.\n",h->fmt_label[0],h->fmt_label[1],h->fmt_label[2],h->fmt_label[3]);
	
		  printf("fmt_size:         %u.\n",h->fmt_size);
		  printf("audio_format:     %u.\n",h->audio_format);
		  printf("channel_count:    %u.\n",h->channel_count);
		  printf("sampling_rate:    %u.\n",h->sampling_rate);
		  
		  printf("bytes_per_second: %u.\n",h->bytes_per_second);
		  printf("bytes_per_sample: %u.\n",h->bytes_per_sample);
		  printf("bits_per_sample:  %u.\n",h->bits_per_sample);
	if(i) printf("data_label:       %c%c%c%c.\n",h->data_label[0],h->data_label[1],h->data_label[2],h->data_label[3]);
		  printf("data_size:        %u.\n",h->data_size);
	
	if(h->bits_per_sample == 16)
	{
		short* p = (short*)data;
		for(unsigned i=90;i<110;++i)
			printf("%d, ",p[i]);
		printf("\n");
	}
	else if(h->bits_per_sample == 8)
	{	
		char* p = (char*)data;
		for(unsigned i=0;i<4;++i)
			printf("%d, ",p[i]);
		printf("\n");
	}
}


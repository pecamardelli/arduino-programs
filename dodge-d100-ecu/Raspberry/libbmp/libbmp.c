/* Copyright 2016 - 2017 Marc Volker Dickmann
 * Project: LibBMP
 */

#include "../ecu.h"

// BMP_HEADER

bmp_header *bmp_header_read (FILE *img_file)
{
	if (img_file == NULL)
	{
		printf("El archivo de imagen no es valido.\n");
		return NULL;
	}
	
	// Since an adress must be passed to fread, create a variable!
	unsigned short magic;
	
	// Check if its an bmp file by comparing the magic nbr:
	if (fread (&magic, sizeof (magic), 1, img_file) != 1 || magic != BMP_MAGIC)
	{
		printf("El archivo de imagen no tiene un numero magico valido.\n");
		return NULL;
	}
	/*
	printf("magic readed. header size: %d...\n", sizeof (bmp_header));
	bmp_header *test	= (bmp_header *)malloc(sizeof(bmp_header));
	fread (test, sizeof (bmp_header), 1, img_file);
	show_header(test);
	free(test);
	*/
	bmp_header *header	= (bmp_header *)malloc(sizeof(bmp_header));
	
	if(!header){
		printf("No se pudo asignar memoria para la cabecera.\n");
		return NULL;
	}
	
	if (fread (header, sizeof (bmp_header), 1, img_file) != 1)
	{
		printf("No se pudo leer la cabecera.\n");
		return NULL;
	}
	return header;
}

void bmp_img_alloc (bmp_img *img)
{
	const size_t h = abs (img->img_header->biHeight);
	
	// Allocate the required memory for the pixels:
	img->img_pixels = malloc (sizeof (bmp_pixel*) * h);
	
	for (size_t y = 0; y < h; y++)
	{
		img->img_pixels[y] = malloc (sizeof (bmp_pixel) * img->img_header->biWidth);
	}
}

void bmp_img_free (bmp_img *img)
{
	const size_t h = abs (img->img_header->biHeight);
	
	for (size_t y = 0; y < h; y++)
	{
		free (img->img_pixels[y]);
	}
	free (img->img_pixels);
	free (img->img_header);
	free (img);
}

bmp_img *bmp_img_read (const char *filename)
{
	
	FILE *img_file	= fopen (filename, "rb");
	
	if (img_file == NULL)
	{
		printf("No se pudo abrir el archivo.\n");
		return NULL;
	}

	bmp_img	*img	= (bmp_img*)malloc(sizeof(bmp_img));
	
	if(!img){
		printf("No se pudo asignar memoria para la imagen.\n");
		fclose (img_file);
		return NULL;
	}
	
	img->img_header	= bmp_header_read(img_file);
	
	if(!img->img_header){
		printf("No se pudo asignar memoria para el header.\n");
		fclose (img_file);
		return NULL;
	}
	
	bmp_img_alloc(img);
	
	// Select the mode (bottom-up or top-down):
	const size_t h			= abs (img->img_header->biHeight);
	const size_t offset		= (img->img_header->biHeight > 0 ? h - 1 : 0);
	const size_t padding	= BMP_GET_PADDING (img->img_header->biWidth);
	const size_t items 		= img->img_header->biWidth; // Needed to compare the return value of fread
	
	// Read the content:
	for (size_t y = 0; y < h; y++)
	{
		// Read a whole row of pixels from the file:
		if (fread (img->img_pixels[abs (offset - y)], sizeof (bmp_pixel), items, img_file) != items)
		{
			fclose(img_file);
			printf("No se pudo leer una fila en el archivo de imagen.\n");
			return NULL;
		}
		
		// Skip the padding:
		fseek (img_file, padding, SEEK_CUR);
	}
	
	// NOTE: All good!
	fclose (img_file);
	return img;
}

void show_header(bmp_header *header){
	printf("bfSize:\t\t %d\n"		,header->bfSize);
	printf("bfOffBits:\t %d\n"		,header->bfOffBits);
	printf("biSize:\t\t %d\n"		,header->biSize);
	printf("biWidth:\t %d\n"		,header->biWidth);
	printf("biHeight:\t %d\n"		,header->biHeight);
	printf("biPlanes:\t %d\n"		,header->biPlanes);
	printf("biBitCount:\t %d\n"		,header->biBitCount);
	printf("biCompression:\t %d\n"	,header->biCompression);
	printf("biSizeImage:\t %d\n"	,header->biSizeImage);
	printf("biXPelsPerMeter: %d\n"	,header->biXPelsPerMeter);
	printf("biYPelsPerMeter: %d\n"	,header->biYPelsPerMeter);
	printf("biClrUsed:\t %d\n"		,header->biClrUsed);
	printf("biClrImportant:\t %d\n"	,header->biClrImportant);
}

void draw_image(image _image){
	if (!_image.filename) {
		perror("No image provided");
		return;
	}

	bmp_img *img	= bmp_img_read(_image.filename);

	if (!img) {
		perror("Could not read image.");
		return;
	}
	const size_t		h				= abs (img->img_header->biHeight);
	const size_t		offset			= (img->img_header->biHeight > 0 ? h - 1 : 0);
	//const size_t		padding			= BMP_GET_PADDING (img->img_header->biWidth);
	const size_t		w	 			= img->img_header->biWidth;
	unsigned short		bytes_per_pixel	= vinfo.bits_per_pixel / 8;
	int					x_pos			= 0;
	int					y_pos			= 0;
	int					line_length		= vinfo.xres * bytes_per_pixel;
	
	if(w > vinfo.xres){
		printf("El ancho de la imagen es mayor que la pantalla.\n");
		return;
	}
	
	if(h > vinfo.yres){
		printf("La altura de la imagen es mayor que la pantalla.\n");
		return;
	}
	
	if(_image.x == CENTER_SCREEN){		
		x_pos	= (vinfo.xres - w) / 2;
	}
	else if(_image.x > vinfo.xres - w){
		x_pos	= vinfo.xres - w;
	}
	else {
		x_pos	= _image.x;
	}
	
	x_pos	= x_pos * bytes_per_pixel;
	
	if(_image.y == MIDDLE_SCREEN){		
		y_pos	= (vinfo.yres - h) / 2;
	}
	else if(_image.y > vinfo.yres - h){
		y_pos	= vinfo.yres - h;
	}
	else {
		y_pos	= _image.y;
	}
	
	y_pos	*= line_length;
		bmp_pixel 			*row;

	// Read the content:
	for (size_t i = h; i > 0; i--)
	{		
		row	= img->img_pixels[abs (offset - h + i)];
		
		for(size_t k = 0; k < w; k++){
			if(_image.alpha == NODRAW){
				*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos)		= 0;
				*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 1)	= 0;
				*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 2)	= 0;
				*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 3)	= 255;
			}
			else{
				if(row[k].blue > 0 || row[k].green > 0 || row[k].red > 0){
					if(_image.color == GREEN){
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos)		= row[k].blue;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 1)	= row[k].red;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 2)	= row[k].green;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 3)	= 255;
					}
					else if(_image.color == YELLOW){
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos)		= row[k].blue;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 1)	= row[k].red;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 2)	= row[k].red;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 3)	= 255;
					}
					else if(_image.color == BLUE){
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos)		= row[k].red;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 1)	= row[k].green;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 2)	= row[k].blue;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 3)	= 255;
					}
					else{
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos)		= row[k].blue;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 1)	= row[k].green;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 2)	= row[k].red;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 3)	= 255;
					}
				}
				else {
					if(_image.alpha == 1){
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos)		= 0;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 1)	= 0;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 2)	= 0;
						*(fbp + k*bytes_per_pixel + x_pos + i*line_length + y_pos + 3)	= 255;
					}
				}
			}
			
		}
	}
	
	bmp_img_free(img);
}

void draw_number(float number, int x, int y, unsigned char color, unsigned char alpha, unsigned char size){
	int decimal		= 0;
	int units		= 0;
	int tens		= 0;
	int hundreds	= 0;
	int thousands	= 0;
	char numsize[8];
	
	while(number >= 1000){
		thousands++;
		number -= 1000;
	}
	
	while(number >= 100){
		hundreds++;
		number -= 100;
	}
	
	while(number >= 10){
		tens++;
		number -= 10;
	}
	
	while(number >= 1){
		units++;
		number -= 1;
	}
	
	if(number > 0){
		decimal	= number * 10;
	}

	switch(size){
		case SMALL:		strcpy(numsize,"small"); break;
		case MEDIUM:	strcpy(numsize,"medium"); break;
		case BIG:		strcpy(numsize,"big"); break;
	}
	
	sprintf(digit.filename, "%s%s%s%d_%s.bmp",sys.main_dir,sys.image_dir,sys.number_dir,thousands,numsize);
	digit.x		= x;
	digit.y		= y; 
	digit.color	= color;
	
	if(thousands == 0){
		digit.alpha	= NODRAW;
	}
	else{
		digit.alpha	= alpha;
	}
	
	draw_image(digit);
	
	
	sprintf(digit.filename, "%s%s%s%d_%s.bmp",sys.main_dir,sys.image_dir,sys.number_dir,hundreds,numsize);
	digit.x		= x + BIG_NUMBER_WIDTH;
	digit.y		= y; 
	digit.color	= color;
	
	if(hundreds == 0){
		digit.alpha	= NODRAW;
	}
	else{
		digit.alpha	= alpha;
	}
	
	draw_image(digit);
	
	sprintf(digit.filename, "%s%s%s%d_%s.bmp",sys.main_dir,sys.image_dir,sys.number_dir,tens,numsize);
	digit.x		= x + BIG_NUMBER_WIDTH * 2;
	digit.y		= y; 
	digit.color	= color;
	
	if(tens == 0){
		digit.alpha	= NODRAW;
	}
	else{
		digit.alpha	= alpha;
	}
	
	draw_image(digit);	
	
	sprintf(digit.filename, "%s%s%s%d_%s.bmp",sys.main_dir,sys.image_dir,sys.number_dir,units,numsize);
	digit.x		= x + BIG_NUMBER_WIDTH * 3;
	digit.y		= y; 
	digit.color	= color;
	digit.alpha	= alpha;
	
	draw_image(digit);
	
	sprintf(digit.filename, "%s%s%sdot_%s.bmp",sys.main_dir,sys.image_dir,sys.number_dir,numsize);
	digit.x		= x + BIG_NUMBER_WIDTH * 4;
	digit.y		= y; 
	digit.color	= color;
	digit.alpha	= alpha;
	
	draw_image(digit);
	
	sprintf(digit.filename, "%s%s%s%d_%s.bmp",sys.main_dir,sys.image_dir,sys.number_dir,decimal,numsize);
	digit.x		= x + BIG_NUMBER_WIDTH * 4 + BIG_DOT_WIDTH;
	digit.y		= y; 
	digit.color	= color;
	digit.alpha	= alpha;
	
	draw_image(digit);
	
}
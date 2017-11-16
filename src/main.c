#include "util.h" // header with things that the entire program needs
#include "read.h" // header with only read things
#include "process.h" // header with only process things
#include "write.h" // header with only write things

// Defines the DEBUG mode on/off (0/1)
#define DEBUG 0

int main(int argc, char const *argv[])
{
	char *filepath = NULL;
	char *format = NULL;
	char *diagFile = NULL;
	// test if all the arguments were provided
	if (argc < 3 && strcmp(argv[1], "-i")) {
		fprintf(stderr, "usage: ./catarata -i <input-image> -f <input-image-format>"
			 " -o <diagnose-file>\n\n\t-i\tspecify the path to the image to be analised"
			 "\n\t-f\tthe format of the input image\n\t-o\tpath to the diagnosis"
			 " file\n\nExample of usage:\n"
			 "./catarata -i res/Catarata.ppm -f ppm -o tests/Catarata_Diag.ppm\n\n");
		exit(1);
	} else if (argc < 7) {
		bool input_bool = false;
		bool format_bool = false;
		bool diagFile_bool = false;
		uchar i = 0;
		while (i < argc) {
			if (!strcmp(argv[i], "-i")) {
				if (i+1 == argc || !strcmp(argv[i+1], "-f") || !strcmp(argv[i+1], "-i")) {
					fprintf(stderr, "usage: ./catarata -i <input-image> -f <input-image-format>"
						 " -o <diagnose-file>\n\n\t-i\tspecify the path to the image to be analised"
						 "\n\t-f\tthe format of the input image\n\t-o\tpath to the diagnosis"
						 " file\n\nExample of usage:\n"
						 "./catarata -i res/Catarata.ppm -f ppm -o tests/Catarata_Diag.ppm\n\n");
					exit(1);
				}
				
				filepath = (char *) argv[i+1];
				input_bool = true;
			}

			if (!strcmp(argv[i], "-f")) {
				if (i+1 == argc || !strcmp(argv[i+1], "-o") || !strcmp(argv[i+1], "-i")) {
					fprintf(stderr, "usage: ./catarata -i <input-image> -f <input-image-format>"
						 " -o <diagnose-file>\n\n\t-i\tspecify the path to the image to be analised"
						 "\n\t-f\tthe format of the input image\n\t-o\tpath to the diagnosis"
						 " file\n\nExample of usage:\n"
						 "./catarata -i res/Catarata.ppm -f ppm -o tests/Catarata_Diag.ppm\n\n");
					exit(1);
				}

				format = (char *) argv[i+1];
				format_bool = true;
			}

			if (!strcmp(argv[i], "-o")) {
				if (i+1 == argc || !strcmp(argv[i+1], "-f") || !strcmp(argv[i+1], "-i")) {
					fprintf(stderr, "usage: ./catarata -i <input-image> -f <input-image-format>"
						 " -o <diagnose-file>\n\n\t-i\tspecify the path to the image to be analised"
						 "\n\t-f\tthe format of the input image\n\t-o\tpath to the diagnosis"
						 " file\n\nExample of usage:\n"
						 "./catarata -i res/Catarata.ppm -f ppm -o tests/Catarata_Diag.ppm\n\n");
					exit(1);
				}

				diagFile = (char *) argv[i+1];
				diagFile_bool = true;
			}

			++i;
		}

		if (!input_bool) {
			fprintf(stderr, "Please specify an input image after the -i.\n");
			exit(1);
		}

		if (!format_bool) {
			printf("\n\e[1m\x1b[33mWARNING\e[0m\x1b[0m: No format specified, defaulting to 'ppm'.\n");
			format = "ppm";
		}

		if (!diagFile_bool) {
			printf("\e[1m\x1b[33mWARNING\e[0m\x1b[0m: No diagnosis file specified, defaulting to 'diagnosis.txt'.\n\n");
			diagFile = "diagnosis.txt";
		}

		if (format_bool && argc%2 == 0) {
			fprintf(stderr, "Please specify a format after the -f.\n");
			exit(1);
		} else if (diagFile_bool && argc%2 == 0) {
			fprintf(stderr, "Please specify a diagnose file after the -o.\n");
			exit(1);
		}
	} else if (argc == 7) {
		/* check what are the args passed on to the program and store the args'
		indexes on a variable, so we can access them any time we want */
		for (uchar i = 0; i < argc; ++i) {
			if (!strcmp(argv[i], "-i")) {
				filepath = (char *) argv[i+1];
			} else if (!strcmp(argv[i], "-f")) {
				format = (char *) argv[i+1];
			} else if (!strcmp(argv[i], "-o")) {
				diagFile = (char *) argv[i+1];
			}
		}
	}

	// little welcome interface I did while bored
	puts("     ---------------------------------------------------------------------");
	puts("    |  _     _  _______  ___      _______  _______  __   __  _______  __  |");
	puts("    | | | _ | ||       ||   |    |       ||       ||  |_|  ||       ||  | |");
	puts("    | | || || ||    ___||   |    |       ||   _   ||       ||    ___||  | |");
	puts("    | |       ||   |___ |   |    |       ||  | |  ||       ||   |___ |  | |");
	puts("    | |       ||    ___||   |___ |      _||  |_|  ||       ||    ___||__| |");
	puts("    | |   _   ||   |___ |       ||     |_ |       || ||_|| ||   |___  __  |");
	puts("    | |__| |__||_______||_______||_______||_______||_|   |_||_______||__| |");
	puts("    |                                                                     |");
	puts("     ---------------------------------------------------------------------");
	puts("___                                                               _             ");
	puts(" | _   |_|_  _   _ _ |_ _  _ _  _|_   _|. _  _  _  _  _. _   _ _ (_|_    _  _ _ ");
	puts(" |(_)  |_| )(-  (_(_||_(_|| (_|(_|_  (_||(_|(_)| )(_)_)|_)  _)(_)| |_\\)/(_|| (- ");
	puts("                                            _/                                  ");
	puts("\n\n /\\   _|_|_  _  _ _.");
	puts("/~~\\|_|| | |(_)| _\\.");
	puts("\n - Felipe C. Ramos Filho\n - João Pedro de A. Paula\n\n\n");

	// these are just auxiliary variables
	char *filename = stripFilepath(filepath);
	char *outDir = "test/";

	// these are some testing printf's, maybe they'll get changed
	printf("The image to be analised is '%s'.\n", filename);
	printf("%s's format is '%s'\n", filename, format);
	printf("The diagnosis is in '%s'.\n", diagFile);

	putchar('\n');

	Img *originalImg = readPPM(filepath);
	// TODO different reading and writing functions for different formats
	
	if (!originalImg) {
		perror(filepath);
		exit(1);
	}

	// here we enter on process.c
	puts("\nStarted processing the image...\n");

	// tone the image to its greyscale
	Img *greyscaled = greyscale(originalImg);

	char *outGrey = outFilepath(outDir, filename, "_grey", format);
	char *strippedGrey = stripFilepath(outGrey);
	if (!greyscaled) {
		fprintf(stderr, "Error writing the greyscaled image to '%s'.\n", strippedGrey);
		freeImg(originalImg);
	} else {
		writePPM(greyscaled, outGrey);
	}
	free(outGrey);
	free(strippedGrey);

	// blur the image with the gaussian filter
	Img *gaussImg = gaussianFilter(greyscaled, 1);

	char *outGauss = outFilepath(outDir, filename, "_gauss", format);
	char *strippedGauss = stripFilepath(outGauss);
	if (!gaussImg) {
		fprintf(stderr, "Error writing the blurred image to '%s'.\n", strippedGauss);
	} else {
		writePPM(gaussImg, outGauss);
	}
	free(outGauss);
	free(strippedGauss);
	
	// apply Sobel's filter to enhance the edges
	Img *sobelImg = sobelFilter(gaussImg, 1);

	char *outSobel = outFilepath(outDir, filename, "_sobel", format);
	char *strippedSobel = stripFilepath(outSobel);
	if (!sobelImg) {
		fprintf(stderr, "Error writing the edge detection image to '%s'.\n", strippedSobel);
	} else {
		writePPM(sobelImg, outSobel);
	}
	free(outSobel);
	free(strippedSobel);


	// Apply threshold to sobelImg with intensity 120 (pixels bigger than this will be converted to 255, smaller than will be 0);
	Img *thresholdImg = threshold(sobelImg, 36);

	char *outThreshold = outFilepath(outDir, filename, "_threshold2", "pbm");
	char *strippedThreshold = stripFilepath(outThreshold);
	if (!thresholdImg){
		fprintf(stderr, "Error writing the threshold increase image to '%s'.\n", strippedThreshold);
	} else {
		writePBM(thresholdImg, outThreshold);
	}
	free(outThreshold);
	free(strippedThreshold);

	freeImg(thresholdImg);
	free(filename);
	return 0;
}

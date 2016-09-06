
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

//static char *device = "default";
//char *buffer;
static char *device = "hw:0,0";                        /* playback device */

snd_pcm_format_t format = 2;
int NUM_FRAMES = 5;
int FRAME_SIZE = 1024*16;
unsigned char buffer[50*1024*16*20];                          /* some random data */
int mult = 4;
void initialize(snd_pcm_t *capture_handle) {

	  int err;
	    if ((err = snd_pcm_set_params(capture_handle,
	                                  format,
	                                  SND_PCM_ACCESS_RW_INTERLEAVED,
	                                  2,
									  44100,
	                                  0,
	                                  50000)) < 0) {   /* 0.5sec */
	            printf("Playback open error: %s\n", snd_strerror(err));
	            exit(EXIT_FAILURE);
	    }
}
void printBuffer(char* b, int start, int length) {
	int i;
	for(i=start; i<start+length; i++) {
		printf("%02x ", b[i]);
	}
	printf("\n");
}
//  TODO: why cant capture anything
void capture(int mode) {
	int i;
	  int err;
	  snd_pcm_t *capture_handle;

	  if ((err = snd_pcm_open (&capture_handle, "hw:0,0", SND_PCM_STREAM_CAPTURE, mode)) < 0) {
	    fprintf (stderr, "cannot open audio device %s (%s)\n",
	             device,
	             snd_strerror (err));
	    exit (1);
	  }

	  initialize(capture_handle);

	  int j;
	  for(j=0; j<sizeof(buffer); j++) buffer[j] = 0;
	  for (i = 0; i < NUM_FRAMES; i++) {

//		  for (j = FRAME_SIZE*i*mult; j < FRAME_SIZE*i*mult*(i+1); j++)
//				  buffer[j] = random() & 0xff;
	    if ((err = snd_pcm_readi (capture_handle, buffer+FRAME_SIZE*i*mult, FRAME_SIZE)) != FRAME_SIZE) {
	      fprintf (stderr, "read from audio interface failed (%s)\n",
	               err, snd_strerror (err));
	      exit (1);
	    }
	    fprintf(stdout, "read %d done\n", i);
		  printBuffer(buffer, FRAME_SIZE*i*mult, 10);


//		    if ((err = snd_pcm_readi (capture_handle, buffer, FRAME_SIZE)) != FRAME_SIZE) {
//		      fprintf (stderr, "read from audio interface failed (%s)\n",
//		               err, snd_strerror (err));
//		      exit (1);
//		    }
		    fprintf(stdout, "read %d done\n", i);
	  }

	  printBuffer(buffer, 0, 10);

	  snd_pcm_close (capture_handle);
	  fprintf(stdout, "audio interface closed\n");
}
unsigned char randomBuffer[50*1024*16*20];                          /* some random data */

void playbackRandom() {
	 int err;
	    unsigned int i;
	    snd_pcm_t *handle;

	    for (i = 0; i < sizeof(randomBuffer); i++)
	    	randomBuffer[i] = random() & 0xff;
	    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 1)) < 0) {
	            printf("Playback open error: %s\n", snd_strerror(err));
	            exit(EXIT_FAILURE);
	    }

	    initialize(handle);

	  printBuffer(randomBuffer, 0, 10);
		for (i = 0; i < NUM_FRAMES; i++) {
			if ((err = snd_pcm_writei (handle, randomBuffer+FRAME_SIZE*i*mult, FRAME_SIZE)) != FRAME_SIZE) {
			  fprintf (stderr, "write from audio interface failed (%s)\n",
					   err, snd_strerror (err));
			  exit (1);
			}
			fprintf(stdout, "write %d done\n", i);
		}
	    snd_pcm_close(handle);
}

void playback() {

    int err;
    unsigned int i;
    snd_pcm_t *handle;

//    for (i = 0; i < sizeof(buffer); i++)
//            buffer[i] = random() & 0xff;
    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            printf("Playback open error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
    }

    initialize(handle);

	  printBuffer(buffer, 0, 10);
//	    for (i = 0; i < sizeof(buffer); i++)
//	    	buffer[i] = random() & 0xff;
	for (i = 0; i < NUM_FRAMES; i++) {
		if ((err = snd_pcm_writei (handle, buffer+FRAME_SIZE*i*mult, FRAME_SIZE)) != FRAME_SIZE) {
		  fprintf (stderr, "write from audio interface failed (%s)\n",
				   err, snd_strerror (err));
		  exit (1);
		}
		fprintf(stdout, "write %d done\n", i);
		printBuffer(buffer, FRAME_SIZE*i*mult, 10);
	}
    snd_pcm_close(handle);

//    int err;
//           unsigned int i;
//           snd_pcm_t *handle;
//           snd_pcm_sframes_t frames;
//           for (i = 0; i < sizeof(buffer); i++)
//                   buffer[i] = random() & 0xff;
//           if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
//                   printf("Playback open error: %s\n", snd_strerror(err));
//                   exit(EXIT_FAILURE);
//           }
//           if ((err = snd_pcm_set_params(handle,
//                                         SND_PCM_FORMAT_U8,
//                                         SND_PCM_ACCESS_RW_INTERLEAVED,
//                                         1,
//                                         48000,
//                                         1,
//                                         500000)) < 0) {   /* 0.5sec */
//                   printf("Playback open error: %s\n", snd_strerror(err));
//                   exit(EXIT_FAILURE);
//           }
//           for (i = 0; i < 16; i++) {
//                   frames = snd_pcm_writei(handle, buffer, sizeof(buffer));
//                   if (frames < 0)
//                           frames = snd_pcm_recover(handle, frames, 0);
//                   if (frames < 0) {
//                           printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
//                           break;
//                   }
//                   if (frames > 0 && frames < (long)sizeof(buffer))
//                           printf("Short write (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
//           }
//           snd_pcm_close(handle);
}

int main(void) {
//	playbackRandom();
	capture(0);
	printf("Capture done, sleeping for 5 secs\n");
	sleep(5);
	playback();
	return 0;
}

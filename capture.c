/*
 *  This extra small demo sends a random samples to your speakers.
 */
#include <alsa/asoundlib.h>
#include <unistd.h>
#include <error.h>
static char *device = "default";                        /* playback device */
snd_output_t *output = NULL;
unsigned char buffer[1024];                          /* some random data */
int main(void)
{
        int err;
        unsigned int i;
        snd_pcm_t *handle;
        snd_pcm_sframes_t frames;
		errno = 0;
        FILE *fp = fopen("/home/oak/workspace/AlsaThird/out.wav", "w");
        if(fp == NULL) {
        	printf("Cant open file: %s \n", strerror(errno));
        	fclose(fp);
        	exit(EXIT_FAILURE);
        }
    	printf("not null");

        if((err = snd_output_stdio_attach(&output, fp, 0)) < 0) {
            printf("Output open error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
        }
        if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
                printf("Playback open error: %s\n", snd_strerror(err));
                exit(EXIT_FAILURE);
        }
        if ((err = snd_pcm_set_params(handle,
                                      SND_PCM_FORMAT_U8,
                                      SND_PCM_ACCESS_RW_INTERLEAVED,
                                      1,
                                      48000,
                                      1,
                                      500000)) < 0) {   /* 0.5sec */
                printf("Playback open error: %s\n", snd_strerror(err));
                exit(EXIT_FAILURE);
        }
        for (i = 0; i < 1; i++) {
        		frames = snd_pcm_readi(handle, buffer, sizeof(buffer));
//                frames = snd_pcm_writei(handle, buffer, sizeof(buffer));
                if (frames < 0)
                        frames = snd_pcm_recover(handle, frames, 0);
                if (frames < 0) {
                        printf("snd_pcm_readi failed: %s\n", snd_strerror(frames));
                        break;
                }
                if (frames > 0 && frames < (long)sizeof(buffer))
                        printf("Short read (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
        		if((err = snd_output_puts(output, buffer)) < 0) {
                    printf("Cant write to file: %s\n", snd_strerror(err));
                    exit(EXIT_FAILURE);
        		}
        }
        fclose(fp);
        snd_pcm_close(handle);
        return 0;
}

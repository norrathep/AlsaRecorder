#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

static int (*wrap_snd_pcm_open)(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode) = NULL;
static snd_pcm_sframes_t (*wrap_snd_pcm_writei)(snd_pcm_t *pcm, const void *buffer, snd_pcm_uframes_t size) = NULL;
static int (*wrap_snd_pcm_set_params) (snd_pcm_t *pcm, snd_pcm_format_t format, snd_pcm_access_t access, unsigned int channels, unsigned int rate, int soft_resample, unsigned int latency) = NULL;
static int (*wrap_snd_pcm_hw_params_set_format)(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_format_t val) = NULL;

int counter = -100;

void * wrap(void * func,char *name)
{
    char *msg;
    if (func == NULL) {
        func = dlsym(RTLD_NEXT, name);
        fprintf(stderr, "** wrapping %s =>  %p\n", name,func);
        if ((msg = dlerror()) != NULL) {
            fprintf(stderr, "** %s: dlopen failed : %s\n", name,msg);
            exit(1);
        } else {
            fprintf(stderr, "** %s: wrapping done\n",name);
        }
    }
    return func;
}

int snd_pcm_hw_params_set_format(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_format_t val) 
{
    int temp;
    wrap_snd_pcm_hw_params_set_format = wrap(wrap_snd_pcm_hw_params_set_format,"snd_pcm_hw_params_set_format");
    temp = wrap_snd_pcm_hw_params_set_format(pcm,params,val);
    return temp;
}

int snd_pcm_set_params(snd_pcm_t *pcm, snd_pcm_format_t format, snd_pcm_access_t access, unsigned int channels, unsigned int rate, int soft_resample, unsigned int latency) 
{
    int temp;
    printf("FORMAT %d\n", (int)format);
    printf("CHANNEL %d\n", channels);
    printf("RATE %d\n", rate);
    printf("RESAMPLE %d\n", soft_resample);
    printf("LATENCY %d\n", latency);
    wrap_snd_pcm_set_params = wrap(wrap_snd_pcm_set_params,"snd_pcm_set_params");
    temp = wrap_snd_pcm_set_params(pcm,format,access,channels,rate,soft_resample,latency);
    return temp;
}

int snd_pcm_open(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode)
{
    int temp;
    counter = 0;

    wrap_snd_pcm_open = wrap(wrap_snd_pcm_open,"snd_pcm_open");
    temp = wrap_snd_pcm_open(pcm,name,stream,mode);
    fprintf(stderr, "** Calling snd_pcm_open for path:[%s] => fd:[%d]\n",name,temp);
    fflush(stderr);
    return temp;

}

snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm, const void *buffer,
                                 snd_pcm_uframes_t size) 
{
   //printf("size: %ld\n", (long)size);
   //printf("first int: %d\n", *(int*)(buffer));
   int mult = 2;
   //printf("next int: %d\n", *(int*)(buffer+(long)size*mult));
   //printf("size of buffer[0]: %d\n", (int)sizeof(buffer[0]));

   // only record when file start.oak exists
   if(access("start.oak", F_OK) != -1) {
   	FILE *fp = fopen("out.raw", "a+");
   	fwrite(buffer, sizeof(buffer[0]), (long)size*mult, fp);
   	fclose(fp);
   }
   snd_pcm_sframes_t temp;
   wrap_snd_pcm_writei = wrap(wrap_snd_pcm_writei,"snd_pcm_writei");
   temp = wrap_snd_pcm_writei(pcm,buffer,size);
   fflush(stderr);
   return temp;
}

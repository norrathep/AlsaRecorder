# AlsaRecorder

make sure u install libasound2 libasound2-dev

gcc capture.c -lasound -lm // for capture.c latency.c read.c pcm.c pcm_min.c

gcc -o alsa-record-example alsa-record.c -lasound && ./alsa-record-example hw:0 // for alsa-record.c


SRC_FFT = main.c zx_fft.c zx_math.c
SRC_XCORR = xcorr.c zx_fft.c zx_math.c

all:test.fft test.xcorr

test.fft:$(SRC_FFT)
	gcc $^ -o $@

test.xcorr:$(SRC_XCORR)
	gcc $^ -o $@

.PHONY:clean run
run:
	-./test.fft > test_fft.txt
	-./test.xcorr > test_xcorr.txt
clean:
	-rm -f test.fft test_fft.txt test_xcorr.txt


#!/bin/bash
printf '\x00\x00\x2E\x23\x00\x00\x2E\x23' | dd of=out.bmp bs=1 seek=38 count=8 conv=notrunc

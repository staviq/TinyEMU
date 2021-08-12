#!/bin/bash

sys="$(uname -s)"
case "${sys}" in
	Linux*)
		./temu $1 diskimage-linux-riscv-2018-09-23/gst-root-riscv64.cfg
		;;
	Darwin*)
		./temu $1 diskimage-linux-riscv-2018-09-23/gst-root-riscv64.cfg
		;;
	CYGWIN*)
		./temu.exe $1 diskimage-linux-riscv-2018-09-23/gst-root-riscv64.cfg
		;;
	MINGW*)
		./temu.exe $1 diskimage-linux-riscv-2018-09-23/gst-root-riscv64.cfg
		;;
	*)
		./temu $1 diskimage-linux-riscv-2018-09-23/gst-root-riscv64.cfg
esac

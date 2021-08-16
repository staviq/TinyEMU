#!/bin/bash

sys="$(uname -s)"
case "${sys}" in
	Linux*)
		./temu "$@" diskimage-linux-riscv-2018-09-23/gst2.cfg
		;;
	Darwin*)
		./temu "$@" diskimage-linux-riscv-2018-09-23/gst2.cfg
		;;
	CYGWIN*)
		./temu.exe "$@" diskimage-linux-riscv-2018-09-23/gst2.cfg
		;;
	MINGW*)
		./temu.exe "$@" diskimage-linux-riscv-2018-09-23/gst2.cfg
		;;
	*)
		./temu "$@" diskimage-linux-riscv-2018-09-23/gst2.cfg
esac

#
# makefile -- Build and run libc tests
#
# Copyright (C) 2020 Bruno Félix Rezende Ribeiro <oitofelix@gnu.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

.PHONY = clean
CFLAGS = -ggdb3
LDFLAGS = -static
CPPFLAGS = -I../include
srcs = $(wildcard *.c)
bins = ${srcs:.c=}
coms := ${bins:=.com}
doslib = ../libc.a
all : ${coms}
${bins} : ${doslib}
clean :
	${RM} ${coms} ${bins}
%.com : %
	ln --force $< $@
${doslib} :
	${MAKE} -C ${@D} ${@F}

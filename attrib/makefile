#
# makefile -- Build attrib.com
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

.PHONY = all clean
CPPFLAGS = -I../libc/include
LDFLAGS = -static
bins = attrib
coms := ${bins:=.com}
doslib = ../libc/libc.a
all : ${coms}
${bins} : ${doslib}
clean :
	${RM} ${bins} ${coms}
%.com : %
	ln $< $@
${doslib} :
	${MAKE} -C ${@D} ${@F}

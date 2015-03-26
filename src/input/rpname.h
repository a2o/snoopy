/*
 * SNOOPY LOGGER
 *
 * File: snoopy/input/rpname.h
 *
 * Copyright (c) 2015 ajzach@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * SNOOPY INPUT: rpname
 */

char* trim_whitespace(char *str);
char* read_proc_property(int pid, char* prop_name);
int get_parent_pid(int pid);
void get_rpname(int pid, int input);
int snoopy_input_rpname(char *input, char *arg);

/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2020 Bostjan Skufca Jese <bostjan@a2o.si>
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



void printMessage       (const char * const message);

void printDiag          (const char * const message);
void printDiagValue     (const char * const message, const char * const value);
void printInfo          (const char * const message);
void printInfoValue     (const char * const message, const char * const value);
void printNotice        (const char * const message);
void printNoticeValue   (const char * const message, const char * const value);
void printSuccess       (const char * const message);
void printWarning       (const char * const message);
void printWarningValue  (const char * const message, const char * const value);
void printError         (const char * const message);
void printErrorValue    (const char * const message, const char * const value);
void fatalError         (const char * const message);
void fatalErrorValue    (const char * const message, const char * const value);

char *      libsnoopySo_getFilePath ();
char *      libsnoopySo_getFilePathNoCheck ();
void        libsnoopySo_load ();
void *      libsnoopySo_dlsym (char const * const);

const char * etcLdSoPreload_getFilePath ();
char *       etcLdSoPreload_readFile ();
void         etcLdSoPreload_writeFile (char * newContent);
const char * etcLdSoPreload_findNonCommentLineContainingString (const char * content, const char * searchString);
const char * etcLdSoPreload_findEntry (const char * content, const char * entry);

extern const char * g_etcLdSoPreloadPath;
extern const char * g_libsnoopySoPath;
extern void *       g_libsnoopySoHandle;

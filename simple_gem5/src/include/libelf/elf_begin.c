#include <libelf.h>

#include "_libelf.h"
#include <string.h>
#include <stdio.h>

ELFTC_VCSID("$Id: elf_begin.c 2364 2011-12-28 17:55:25Z jkoshy $");

Elf *
elf_begin(int fd, Elf_Cmd c, Elf *a)
{
	Elf *e;

	e = NULL;

	if (LIBELF_PRIVATE(version) == EV_NONE) {
		LIBELF_SET_ERROR(SEQUENCE, 0);
		return (NULL);
	}

	switch (c) {
	case ELF_C_NULL:
		return (NULL);

	case ELF_C_WRITE:
		/*
		 * The ELF_C_WRITE command is required to ignore the
		 * descriptor passed in.
		 */
		a = NULL;
		break;

	case ELF_C_RDWR:
		if (a != NULL) { /* not allowed for ar(1) archives. */
			LIBELF_SET_ERROR(ARGUMENT, 0);
			return (NULL);
		}
		/*FALLTHROUGH*/
	case ELF_C_READ:
		/*
		 * Descriptor `a' could be for a regular ELF file, or
		 * for an ar(1) archive.  If descriptor `a' was opened
		 * using a valid file descriptor, we need to check if
		 * the passed in `fd' value matches the original one.
		 */
		if (a &&
		    ((a->e_fd != -1 && a->e_fd != fd) || c != a->e_cmd)) {
			LIBELF_SET_ERROR(ARGUMENT, 0);
			return (NULL);
		}
		break;

	default:
		LIBELF_SET_ERROR(ARGUMENT, 0);
		return (NULL);

	}
    //printf("a = %d",a->e_kind);
	if (a == NULL)
		e = _libelf_open_object(fd, c, 1);
	else if (a->e_kind == ELF_K_AR)
		e = _libelf_ar_open_member(a->e_fd, c, a);
	else
		(e = a)->e_activations++;

	return (e);
}

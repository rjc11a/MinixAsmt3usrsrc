/*
 * This file was generated by the mkinit program.
 */

#include "shell.h"
#include "mystring.h"
#include "alias.h"
#include "eval.h"
#include "exec.h"
#include "input.h"
#include "error.h"
#include <sys/types.h>
#include <stdlib.h>
#include "options.h"
#include "redir.h"
#include <signal.h>
#include "trap.h"
#include "var.h"



#undef  ATABSIZE
#define ATABSIZE 39
#undef  _PATH_STDPATH
#define _PATH_STDPATH "/usr/bin:/bin:/usr/sbin:/sbin:"
#undef  EV_EXIT
#define EV_EXIT 01		/* exit after evaluating tree */
#undef  EV_TESTED
#define EV_TESTED 02		/* exit status is checked; ignore -e flag */
#undef  EV_BACKCMD
#define EV_BACKCMD 04		/* command executing within back quotes */
#undef  CMDTABLESIZE
#define CMDTABLESIZE 31		/* should be prime */
#undef  ARB
#define ARB 1			/* actual size determined at run time */
#undef  EOF_NLEFT
#define EOF_NLEFT -99		/* value of parsenleft when EOF pushed back */
#undef  _PATH_TTY
#define _PATH_TTY "/dev/tty"
#undef  _PATH_DEVNULL
#define _PATH_DEVNULL "/dev/null"
#undef  MAXCMDTEXT
#define MAXCMDTEXT	200
#undef  DEFINE_OPTIONS
#define DEFINE_OPTIONS
#undef 	EOFMARKLEN
#define	EOFMARKLEN	79
#undef 	PROMPTLEN
#define	PROMPTLEN	128
#undef  GDB_HACK
#define GDB_HACK 1 /* avoid local declarations which gdb can't handle */
#undef  EMPTY
#define EMPTY -2		/* marks an unused slot in redirtab */
#undef  PIPESIZE
#define PIPESIZE 4096		/* amount of buffering in a pipe */
#undef  NO_SIGINTERRUPT
#define NO_SIGINTERRUPT
#undef  NO_SYS_SIGNAME
#define NO_SYS_SIGNAME
#undef  NO_SYS_SIGLIST
#define NO_SYS_SIGLIST
#undef  S_DFL
#define S_DFL 1			/* default signal handling (SIG_DFL) */
#undef  S_CATCH
#define S_CATCH 2		/* signal is caught */
#undef  S_IGN
#define S_IGN 3			/* signal is ignored (SIG_IGN) */
#undef  S_HARD_IGN
#define S_HARD_IGN 4		/* signal is ignored permanently */
#undef  S_RESET
#define S_RESET 5		/* temporary - to reset a hard ignored sig */
#undef  _PATH_DEFPATH
#define _PATH_DEFPATH "/usr/bin:/bin"
#undef  VTABSIZE
#define VTABSIZE 39



extern int evalskip;		/* set if we are skipping commands */
extern int loopnest;		/* current loop nesting level */

struct strpush {
	struct strpush *prev;	/* preceding string on stack */
	char *prevstring;
	int prevnleft;
	int prevlleft;
	struct alias *ap;	/* if push was associated with an alias */
};

struct parsefile {
	struct parsefile *prev;	/* preceding file on stack */
	int linno;		/* current line */
	int fd;			/* file descriptor (or -1 if string) */
	int nleft;		/* number of chars left in this line */
	int lleft;		/* number of lines left in this buffer */
	char *nextc;		/* next char in buffer */
	char *buf;		/* input buffer */
	struct strpush *strpush; /* for pushing strings at this level */
	struct strpush basestrpush; /* so pushing one is fast */
};

extern int parsenleft;		/* copy of parsefile->nleft */
extern int parselleft;		/* copy of parsefile->lleft */
extern struct parsefile basepf;	/* top level input file */

extern pid_t backgndpid;	/* pid of last background process */
extern int jobctl;

extern int tokpushback;		/* last token pushed back */
extern int checkkwd;            /* 1 == check for kwds, 2 == also eat newlines */

struct redirtab {
	struct redirtab *next;
	int renamed[10];
};

extern struct redirtab *redirlist;

extern char sigmode[_NSIG];	/* current value of signal */



/*
 * Initialization code.
 */

void
init(void) {

      /* from input.c: */
      {
	      extern char basebuf[];

	      basepf.nextc = basepf.buf = basebuf;
      }

      /* from var.c: */
      {
	      char **envp;
	      extern char **environ;

	      initvar();
	      for (envp = environ ; *envp ; envp++) {
		      if (strchr(*envp, '=')) {
			      setvareq(*envp, VEXPORT|VTEXTFIXED);
		      }
	      }
      }
}



/*
 * This routine is called when an error or an interrupt occurs in an
 * interactive shell and control is returned to the main command loop.
 */

void
reset(void) {

      /* from eval.c: */
      {
	      evalskip = 0;
	      loopnest = 0;
	      funcnest = 0;
      }

      /* from input.c: */
      {
	      if (exception != EXSHELLPROC)
		      parselleft = parsenleft = 0;	/* clear input buffer */
	      popallfiles();
      }

      /* from parser.c: */
      {
	      tokpushback = 0;
	      checkkwd = 0;
      }

      /* from redir.c: */
      {
	      while (redirlist)
		      popredir();
      }
}



/*
 * This routine is called to initialize the shell to run a shell procedure.
 */

void
initshellproc(void) {

      /* from alias.c: */
      {
	      rmaliases();
      }

      /* from eval.c: */
      {
	      exitstatus = 0;
      }

      /* from exec.c: */
      {
	      deletefuncs();
      }

      /* from input.c: */
      {
	      popallfiles();
      }

      /* from jobs.c: */
      {
	      backgndpid = -1;
#if JOBS
	      jobctl = 0;
#endif
      }

      /* from options.c: */
      {
	      int i;

	      for (i = 0; i < NOPTS; i++)
		      optlist[i].val = 0;
	      optschanged();

      }

      /* from redir.c: */
      {
	      clearredir();
      }

      /* from trap.c: */
      {
	      char *sm;

	      clear_traps();
	      for (sm = sigmode ; sm < sigmode + _NSIG ; sm++) {
		      if (*sm == S_IGN)
			      *sm = S_HARD_IGN;
	      }
      }

      /* from var.c: */
      {
	      shprocvar();
      }
}

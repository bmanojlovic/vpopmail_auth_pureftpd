#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include<vpopmail.h>
#include<stdio.h>
#include<vauth.h>
#include<unistd.h>

typedef struct vpop_quota
{
  int q_size;
  int q_count;
}
vpop_quota;

static void *
parse_quota (char *quota_raw)
{
  char buf1[50];
  char buf2[50];
  int size, count, b1, b2;

  struct vpop_quota *quota =
    (struct vpop_quota *) malloc (sizeof (vpop_quota));

  if (!strcmp (quota_raw, "NOQUOTA"))
    return NULL;
  sprintf (buf1, strdup (strtok (quota_raw, ",")));
  sprintf (buf2, strdup (strtok (NULL, ",")));

  b1 = strlen (buf1);
  b2 = strlen (buf2);
  if (strcmp (buf1, "S"))
    {
      buf1[b1 - 1] = '\0';
      quota->q_size = atoi (buf1);
    }
  else
    {
      buf1[b1 - 1] = '\0';
      quota->q_count = atoi (buf1);
    }
  if (strcmp (buf2, "C"))
    {
      buf2[b2 - 1] = '\0';
      quota->q_count = atoi (buf2);
    }
  else
    {
      buf2[b2 - 1] = '\0';
      quota->q_size = atoi (buf2);
    }
  return quota;
}

main (int argc, char *argv[])
{

  struct vqpasswd *vpw;
  struct vpop_quota *quota =
    (struct vpop_quota *) malloc (sizeof (vpop_quota));
  char *luzer_mail;
  char *luzer_user;
  char *luzer_domain;
  char *luzer_password;

  if ((luzer_mail = getenv ("AUTHD_ACCOUNT")) == NULL)
    {
      printf ("auth_ok:0\n");
      exit (-1);
    };
  if ((luzer_password = getenv ("AUTHD_PASSWORD")) == NULL)
    {
      printf ("auth_ok:0\n");
      exit (-1);
    };

  luzer_user = strdup (strtok (luzer_mail, "@"));
  luzer_domain = strdup (strtok (NULL, "@"));

  vpw = vauth_getpw (luzer_user, luzer_domain);

  quota = parse_quota (vpw->pw_shell);
  printf ("auth_ok:1\n");
  printf ("uid:507\n");
  printf ("gid:502\n");
  printf ("user_quota_size:%i\n", quota->q_size);
  printf ("user_quota_files:%i\n", quota->q_count);
  printf ("per_user_max:2\n");
  printf ("dir:%s\n", vpw->pw_dir);
  printf ("end\n");
}

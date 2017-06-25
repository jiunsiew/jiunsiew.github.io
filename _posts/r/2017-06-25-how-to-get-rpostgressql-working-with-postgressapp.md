---
layout: post
title: "How to get RPostgresSQL working with Postgres.app"
subtitle: "and the blessings and curses of open source software"
modified:
categories: r
excerpt: "Getting RPostgresSQL working with Postgres.app on macOS was a real eye opener on the pros and cons of open source software."
tags: [R, SQL, Postgres]
comments: true
share: true
image:
  feature:
date: 2017-06-25
---
  
## Introduction
On a recent whim, I decided to upgrade to the latest version of `dplyr` to version 0.7.0 without doing much prior reading into the changes that were made.  As a result, I very quickly discovered the database connections that were once in-built had been abstracted to another package called `dbplyr`, resulting in some strange errors.  "No worries", I thought as I then just installed that package.  

Re-running some code that connected to a local Postgres database provided some very frustrating errors.  "No worries", I thought to myself again, and proceeded to then upgrade my version of R to version 3.4.0, only to be met with the same frustrating errors of not being able to connect to my database due to some errors with the `RPostgresSQL` package.  

I was eventually able to get everything back up and running but only after having to dig into the source code of `RPostgresSQL` and recompiling it.  The steps I had to go through are outlined below.

Reflecting on this excercise (or ordeal, if you will), it dawned on me that this was a really good example of the benefits and drawbacks of open source software and thought I'd opine a bit on this issue.  

But first, the technical bits...

## When all else fails, look at the source
If you happen to use macOS and want to use Postgres, you'll very quickly find that [Postgres.app](https://postgresapp.com) is a very handy way to get started.  It worked pretty much out-of-the-box with previous versions of `RPostgresSQL` and `dplyr`.  I have no idea what changed but with `dplyr` version 0.7.0 and the abstraction of the database connections to `dbplyr`, the out-of-the-box functionality broke with `RPostgresSQL`.  

### What's gone wrong?  
Basically, `RPostgresSQL` was unable to find the Postgres executable files.  By default, `RPostgresSQL` will search for the Postgres applications in `/usr/lib` or `/opt/lib` directories.  The problem is that by default, `Postgres.app` is installed in the `/Applications` directory which is not in the search path of `RPostgresSQL`.  The diagnosis of this problem was probably the hardest and most frustrating part but one clue did come from this [blog post by Ian Gow](https://iangow.wordpress.com/2011/04/23/installing-rpostgresql-with-r-2-13-on-a-mac/).  

### So, how do you fix it?  
Well, *a* solution is to modify the source code, making sure that the path is added appropriately, and recompile the package.  This is actually not as hard as it sounds and I was able to get everything working as before.  Here are the steps I took to do that:

   * First, download the source code from [CRAN](https://cran.r-project.org/web/packages/RPostgreSQL/index.html).  You'll want to download the `*.tar.gz` source code file.  Save it to a local directory.  I saved it to the default `~/Downloads` directory on my home drive.
   * Then, fire up the Terminal and uncompress the file with the command: `tar xvzf RPostgreSQL_0.4-1.tar.gz` (I got version 0.4.1 which seems to have already been updated to v0.6.2 as of 2017-06-24).
   * You now need to open up the `configure` file in the uncompressed directory.
   * Look for the following bits of code:
   

{% highlight bash %}
for dir in \
	/usr/include \
	/usr/include/pgsql \
	/usr/include/postgresql \
	/usr/local/include \ ...
{% endhighlight %}
   
   * Once you've found that (it was around line 2915 of the code version I got), add the location of the `Postgres.app` to the end of the list.  I found two such loops and added the new location to both.


{% highlight bash %}
for dir in \
	/usr/include \
	/usr/include/pgsql \
	/usr/include/postgresql \
	/usr/local/include \
	...
	/Applications/Postgres.app/Contents/Versions/latest/include
	do 
 ...
{% endhighlight %}
   
   * Finally, re-install the R package with the newly modified file using the command:


{% highlight r %}
install.packages("~/Downloads/RPostgreSQL", repos=NULL, type="source")
{% endhighlight %}

   * Problem solved.
   
For me, the changes above were enough for me to get database connections working again.  However, the comments to Ian Gow's blog post above does provide a link to the `RPostgresSQL` [issue tracker](https://code.google.com/archive/p/rpostgresql/issues/30) where there is another suggested fix which involves 

>  defin[ing] the full path for pg_config in the environment variable $PG_CONFIG...

I didn't try this as I was happy enough with the solution I got.  Hope that helps...someone.



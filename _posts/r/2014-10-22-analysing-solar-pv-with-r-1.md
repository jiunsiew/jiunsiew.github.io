---
layout: post
title: "Analysing solar PV with R Part 1: Setup"
modified:
categories: r
excerpt: "The first part in a series on the process of data extraction and cleaning using solar PV data from the Australian Clean Energy Regulator."
tags: [R, data munging]
comments: true
share: true
image:
  feature:
date: 2014-10-22
---

Doing data analysis is quite often much more than just doing fancy algorithms and visualisations.  There's typically a lot of background data minging (no that's not a typo) work that needs to be done, particularly if you're intending to sustain your code over a period of time and share it with others.  I thought I'd write a bit about this whole workflow by doing a series of post analysing the amount of residential rooftop solar PV in Australia.  Of course, I'll be using R. 

For those who might not know, rooftop PV has seen some phenomenal growth Down Under due in large part to government incentive schemes (some ludicrously generous) coinciding with falls in system prices--particularly in the modules themselves. As though that wasn't enough, electricity prices have skyrocketed around the same time giving people even more incentive to reduce their usage from the grid.

In this post, I'll focus on setting up a project structure that I typically like to use when setting up a workflow in R.  Future posts will focus on getting new data from Excel into R, and tidying it up with `tidyr` in a way that makes things easier to update and maintain, and finally do some ongoing analysis on the data we import.  

If you're interested in running the code yourself, all the source code and data files are up on 
[Github](https://github.com/jiunsiew/pv)


### Setting up a directory structure

Organisation plays a really important role in any software development project.  I typically set up a project in R Studio and have this linked to Github for version control.  I won't go into version control here other than to say that you should really use it (try GitHub or Bitbucket).  Adding this discipline at the start of any project helps if you need to modify the code later or share it with someone else.  At the very least, version control gives you a backup copy should something catastrophic happen to your machine.

As is typical with R, there are packages that help you start up a project structure such as 
[Project Template](http://cran.r-project.org/web/packages/ProjectTemplate/index.html), but I've found it's a bit too specific.  I tend to like keeping things simple, so my directory structure just consists of something like `source`, `test` and `data` directories.  I've found this is enough to keep things organised without being too restrictive.  If you're intending to create an R package for distribution down the track then it's worth considering setting up a directory structure that facilitates that now as well.  

Within these directories, I usually split them out by function.  In this case, the data that I'm going to use can be found from the [Clean Energy Regulator (CER) here](http://ret.cleanenergyregulator.gov.au/REC-Registry/Data-reports). I'll also be looking at some other data from the Reserve Bank and possibly adding some geolocation data later-so each will have their own subdirectories.

The picture below shows the directory structure of how I like to keep the project structured. Under the project directory, I keep a data folder that is then divided into several data types; `installedCap` being the directory where I store the rooftop PV installed capacity data from the CER. The original Excel files are stored in the `raw/original` directory.

![](/images/2014-10-22-analysing-solar-pv-with-r-1/pv_dirStructure.jpg)

The CER data is updated approximately every month where the installed capacity values are updated with each report. So, for say the month of April 2014, the installed capacity numbers will most likely vary between the reports published in May and August.  I've found that it's always a good idea to keep an unadulterated copy of the data in it's original form. This way, it's always there should you need to refer back to it for whatever reason--bugs being the most common one.  

The same philosophy can be applied to the source directory.  Test scripts are obviously stored in the test directory with the intention of unit testing functions and scripts.  

### Recap 

- Spend a bit of time thinking how you want to set up your project before you get started.  It will save you a lot of headaches later.
- Keep a copy of whatever original data you have.  Make copies of them as you clean them but don't overwrite the original dataset.
- Version control your code.  It's free and fairly easy and at least you'll have a backup copy of your work if anything happens to your machine.

Once all this is setup, the next step is to get the data into R.  I'll cover this I'm the next post.

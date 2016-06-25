---
layout: post
title: "Analysing solar PV with R Part 2: Data Munging"
modified:
categories: r
excerpt: "The second part in a series on the process of data extraction and cleaning using solar PV data from the Australian Clean Energy Regulator."
tags: [R, data munging]
comments: true
share: true
image:
  feature:
date: 2014-11-01
---

### Flashback
This is the second post in a series on analysing rooftop PV solar using R.  In the 
[first post](r/analysing-solar-pv-with-r-1/), I spilt a lot of digital ink on how to setup the project.  A slow start perhaps, but in this post, we'll look at something more directly R related. 

In particular, I'll cover how to get data from Excel into R, and tidy it up into a format that makes things easier to analyse later.  To do this, I'll be using Hadley Wickham's [`tidyr`](http://cran.r-project.org/web/packages/tidyr/index.html) package.

Again, if you're interested in running the code yourself, all the source code and data files are up on [Github](https://github.com/jiunsiew/pv).


### Out of Excel and into R: automating the process

I haven't really scripted the downloading of the [latest excel data files from the CER](http://ret.cleanenergyregulator.gov.au/REC-Registry/Data-reports#Latest-data) mainly because the frequency of updating it is so low (once a month), it's probably easier to 'right-click-and-save'--which is what I've done.  

One complication to the dataset that we're working with is that the Excel file formats have changed over time making it harder to develop just one script for all the files. So you'll have to either manually strip it out or do it programatically for each data file you get.  I've done one example here using the [XLConnect](http://cran.r-project.org/web/packages/XLConnect/index.html) package.  It's quite a handy package because it allows you to specify the cell range of the data set you want to import and the worksheet you want to read by using only the `readWorksheet` function.  This is particularly relevant to the Excel files I've downloaded since the first few rows of the spreadsheet are fluff text and there are a few other worksheets that contain other data which I'm not particularly interested in.  Once the data's loaded into R, I only keep the columns that I want and save the file to a csv.  

Here's the code that does that:


{% highlight r %}
library(XLConnect)
workBook <- loadWorkbook(filename = "/data/installedCap/raw/original/RET-data-0914.xls")
solarData <- readWorksheet(workBook, sheet = "SGU - Solar Panels", startRow = 4)

# get rid of the previous years columns (cols 2 and 3)
# get rid of the total columns (cols 44 and 45)
solarData <- subset(solarData, select = c(-2,-3, -44, -45))

# save to csv
write.csv(solarData,
          file = "./data/installedCap/raw/pv_201301-201408_ret201409.csv",
          row.names = FALSE)
{% endhighlight %}

Here's what the first few columns of `solarData` dataframe look like:


{% highlight r %}
#   Small.Unit.Installation.Postcode Jan.2013...Installations.Quantity Jan.2013...SGU.Rated.Output.In.kW
# 1                             0000                                 0                              0.00
# 2                             0200                                 0                              0.00
# 3                             0800                                 1                              4.56
# 4                             0801                                 0                              0.00
# 5                             0804                                 0                              0.00
{% endhighlight %}

### Tidy data

The csv data now only contains the relevant data--all the other extraneous text has been stripped.  But, it's still hard to use in the format that it's in, where every column contains similar information (installed capacity and number of installations) but for different monthly periods.  A more useful format would be to have only three variables (postcode, time, installed capacity, and number of installations)--a [tidy](vita.had.co.nz/papers/tidy-data.pdf) format.  

A really easy way to do this is with Hadley Wickham's [tidyr](http://cran.r-project.org/web/packages/tidyr/index.html) package.  Here are some code snippets that does that (you can see the source code in the `getInstalledCapacity.R` script on the [Github repository](https://github.com/jiunsiew/pv/blob/master/src/getInstalledCapacity.R):


{% highlight r %}
rawData <- read.csv(fileName)

installedCap <- rawData %>% 
    # get rid of the last column because it's a total 
    gather(key, value, -1) %>%  
    # text munging to strip the date and data type (capacity or number of installations)
    separate(key, c("date", "type"), sep = "\\.\\.\\.") %>%  
    spread(type, value) 

# clean up
names(installedCap) <- c("Postcode", "date", "numberInstallations", "totalCapacity_kW") 
{% endhighlight %}

It's amazing that you can do all that in 4 lines of code!   I also add a column specifying which csv file the data is from and change the dates from character to numerical values or POSIX format so it's easier to manipulate later.  This is very easily done with the `dmy` command from the [lubridate](http://cran.r-project.org/web/packages/lubridate/index.html) package:


{% highlight r %}
installedCap$date <- dmy(paste0("1.",installedCap$date))
{% endhighlight %}


### Putting it all together

By making the data tidying steps a function, it's very easy to mix and match different source csv files which makes it ideal for incorporating updated data.  I've used the `ldply` function from Hadley Wickham's `plyr` package to do this (see the `mungeInstalledCap.R` script):


{% highlight r %}
source("./src/getInstalledCapacity.R")

fileDir <- "./data/installedCap/raw"
fileList <- c(file.path(fileDir, "pv_200001-201012_ret201212.csv"),
              file.path(fileDir, "pv_201101-201112_ret201212.csv"),
              file.path(fileDir, "pv_201201-201212_ret201401.csv"),
              file.path(fileDir, "pv_201301-201408_ret201409.csv"))

installedCap <- ldply(fileList, .fun = getInstalledCapacity)
{% endhighlight %}

The first few rows of the final data frame is shown below:


{% highlight r %}
#   Postcode       date numberInstallations totalCapacity_kW                     dataSource
# 1        0 2010-12-01                   4            4.460 pv_200001-201012_ret201212.csv
# 2      200 2010-12-01                   1            0.080 pv_200001-201012_ret201212.csv
# 3      800 2010-12-01                   7           34.740 pv_200001-201012_ret201212.csv
# 4      801 2010-12-01                   2            3.440 pv_200001-201012_ret201212.csv
# 5      804 2010-12-01                   4            7.170 pv_200001-201012_ret201212.csv
{% endhighlight %}

### Postcode mapping

Having just the postcode on it's own is pretty good, but it doesn't really help if we're trying to aggregate this up to a state level.  I managed to download a copy of the postcode definition in csv format from the Australian Post Office last year (it looks like they're charging for it now) and have merged it with the `installedCap` dataframe:


{% highlight r %}
# map some state information to each postcode
postcodeDefn <- read.csv("./data/geography/clean/pc_full_lat_long.csv")

# get rid of duplicates
postcodeDefn <- postcodeDefn[!(duplicated(postcodeDefn$Postcode)), ]

# merge
installedCap <- merge(installedCap, postcodeDefn, by = "Postcode", all.x = TRUE)
{% endhighlight %}

The final data frame now looks like this:


{% highlight r %}
head(installedCap)

#   Postcode       date numberInstallations totalCapacity_kW                     dataSource                        Locality State          Category        Lat     Long
# 1      200 2012-01-01                   0                0 pv_201201-201212_ret201401.csv AUSTRALIAN NATIONAL UNIVERSITY   ACT Post Office Boxes -35.277272 149.1171
# 2      200 2012-03-01                   0                0 pv_201201-201212_ret201401.csv AUSTRALIAN # NATIONAL UNIVERSITY   ACT Post Office Boxes -35.277272 149.1171
# 3      200 2011-01-01                   0                0 pv_201101-201112_ret201212.csv AUSTRALIAN NATIONAL UNIVERSITY   ACT Post Office Boxes -35.277272 149.1171
# 4      200 2012-02-01                   0                0 pv_201201-201212_ret201401.csv AUSTRALIAN NATIONAL UNIVERSITY   ACT Post Office Boxes -35.277272 149.1171
# 5      200 2011-02-01                   0                0 pv_201101-201112_ret201212.csv AUSTRALIAN NATIONAL UNIVERSITY   ACT Post Office Boxes -35.277272 149.1171
# 6      200 2011-05-01                   0                0 pv_201101-201112_ret201212.csv AUSTRALIAN NATIONAL UNIVERSITY   ACT Post Office Boxes -35.277272 149.1171
{% endhighlight %}

### Recap

- `XLConnect` is a really useful package to get data from Excel files into R, particularly if your data files have other worksheets and you only want a targeted range of cells.
- Try to get your data sets into a 'tidy' format.  This helps make analysis a lot easier later.  See Hadley Wickham's [Tidy data paper](vita.had.co.nz/papers/tidy-data.pdf) on this.
- One really good data munging package is [`tidyr`](http://cran.r-project.org/web/packages/tidyr/index.html) by Hadley Wickham.  Do check out his other packages as well such as [`plyr`](http://cran.r-project.org/web/packages/plyr/index.html), [`dplyr`](http://cran.r-project.org/web/packages/dplyr/index.html), and [`reshape2`](http://cran.r-project.org/web/packages/reshape2/index.html)--they really make your code very concise and allow you to focus your mental energy where it should be most--analytics!

And now we're *finally* ready to start some analysis.  

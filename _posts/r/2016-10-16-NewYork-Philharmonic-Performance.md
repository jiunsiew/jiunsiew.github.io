---
layout: post
title: "New York Philharmonic Performances"
modified:
categories: r
excerpt: "A closer look at the NY Philharmonic performances."
tags: [R, data visualisation]
comments: true
share: true
image:
  feature:
date: 2016-10-16
---

## The New York Philharmonic Performance Dataset
One of the really great things about listening to the  [FiveThirtyEight](http://fivethirtyeight.com/tag/whats-the-point/) podcast is 
that you sometimes come across some very interesting data sets.  In one [episode](http://fivethirtyeight.com/features/take-a-trip-through-174-years-of-classical-music/), 
they interviewed Barbara Haws--the archivist/historian, of the New York 
Philharmonic Orchestra from which I learnt that their complete performance
history dating back to 1842 is on 
[Github](https://github.com/nyphilarchive/PerformanceHistory)!

Data and music are two things I really love, so it wasn't long before I started 
digging into the data.  

**A couple of disclamers**:

* I presented some of this material to the Melbourne R Users Meetup group. But 
this post contains a more detailed analysis.  For the presentation please see 
[this link](http://jiunsiew.github.io/plotly_presentation/)

* The data for this post was pulled in April this year, so 2016 data is 
very incomplete

### Data preparation
The performance data is provided in XML files that cover various date ranges.
Since the NY Philharmonic provided a Python script that did some XML extraction, 
I decided to use this as a code base to getting some data fields I was interested
in.  Specifically, I extracted date/time, location, composer and work data and
saved it into a csv which I then imported into R.  I then had to do some extra
cleaning of the data--mainly cleaning up some composer names and the like.  

Here's a quick snapshot of what the data looks like:




{% highlight text %}
## Classes 'tbl_df', 'tbl' and 'data.frame':	82311 obs. of  8 variables:
##  $ programID   : int  3853 3853 3853 3853 3853 3853 3853 3853 3853 5178 ...
##  $ date        : Date, format: "1842-12-07" "1842-12-07" ...
##  $ Time        : chr  "8:00PM" "8:00PM" "8:00PM" "8:00PM" ...
##  $ eventType   : chr  "Subscription Season" "Subscription Season" "Subscription Season" "Subscription Season" ...
##  $ Location    : chr  "Manhattan, NY" "Manhattan, NY" "Manhattan, NY" "Manhattan, NY" ...
##  $ Venue       : chr  "Apollo Rooms" "Apollo Rooms" "Apollo Rooms" "Apollo Rooms" ...
##  $ composerName: chr  "Beethoven,  Ludwig  van" "Weber,  Carl  Maria Von" "Hummel,  Johann" NA ...
##  $ workTitle   : chr  "SYMPHONY NO. 5 IN C MINOR, OP.67" "OBERON" "QUINTET, PIANO, D MINOR, OP. 74" NA ...
{% endhighlight %}



{% highlight text %}
## # A tibble: 6 x 8
##   programID       date   Time           eventType      Location
##       <int>     <date>  <chr>               <chr>         <chr>
## 1      3853 1842-12-07 8:00PM Subscription Season Manhattan, NY
## 2      3853 1842-12-07 8:00PM Subscription Season Manhattan, NY
## 3      3853 1842-12-07 8:00PM Subscription Season Manhattan, NY
## 4      3853 1842-12-07 8:00PM Subscription Season Manhattan, NY
## 5      3853 1842-12-07 8:00PM Subscription Season Manhattan, NY
## 6      3853 1842-12-07 8:00PM Subscription Season Manhattan, NY
## # ... with 3 more variables: Venue <chr>, composerName <chr>,
## #   workTitle <chr>
{% endhighlight %}

There's obviously a bit more to clean up here, but I'll skip those *tiny* 
details and instead focus on the findings.  If you're interested, the code is
all up on [Github](https://github.com/jiunsiew/NY_Philarchive_performanceHistory/tree/master/Scripts).

So, let's have a look at a few things I found interesting.


### Performances over time
One of the immediate questions I was curious to find out was how the number of 
performances might have changed over time.

The conventional wisdom seems to suggest that classical music's popularity has 
been in decline and I wondered if this was reflected in the data.  

Here's a time series of the total number of performances that have been held.

![center](/images/2016-10-16-NewYork-Philharmonic-Performance/unnamed-chunk-3-1.png)

It looks like after a slow start for the first 50 years, the number of 
performances really shoot up to new level from about 1907 (indicated by the red 
dashed line) and peaks about 1925.  A brief check of the NY Philharmonic history 
on [Wikipedia](https://en.wikipedia.org/wiki/New_York_Philharmonic) reveals that 
this was in large part due to a merger with the National Symphony Orchestra in 
1921 and the data certainly reflects this.

Performances seem to trend down between the 1930's to 1960's but given that this 
period encompasses both the Great Depression and the Second World War, I 
actually think it's quite remarkable that the NY Philharmonic performed as 
much as they did.



### Top composers
The second question I had in mind was which composers had the most works 
performed?  I know I've got my favorites, but how would my preferences compare 
to the NY Phil?

Across the entire time range, here are the top 20 most performed composers:

![center](/images/2016-10-16-NewYork-Philharmonic-Performance/unnamed-chunk-4-1.png)

Wagner for the win!  Who would have thought!  Beethoven--one of my favorites--
comes in at a respectable second place with Tchaikovsky coming in third.  Wagner 
really surprised me, as he was primarily an operatic composer but 
nonetheless, some of his works seem very popular.  

Which got me wondering:  which works were most popular?  Here's a plot of the 
top 10 most played works by composer.

![center](/images/2016-10-16-NewYork-Philharmonic-Performance/unnamed-chunk-5-1.png)

Unsurprisingly, Wagner features heavily here with half of the top 10 most 
played pieces being one of his compositions.  Also unsurprising is that Beethoven's
only work in the top ten is his iconic fifth symphony.

Also unexpected but not surprising, is that the American national anthem gets 
played very often, about as much as Berlioz's Damnation of Faust.  Go figure.

Gershwin creeps in the tenth spot here with Porgy and Bess which is rather 
interesting--I'll have to look more into this one.


### Location, location, location
In the podcast, Barbara also mentioned a world tour that the Philharmonic took 
which made the national newspapers at the time.  Being based in Melbourne 
Australia, I thought I'd also have a look at the countries where the NY 
Philharmonic has performed over the years.  


![center](/images/2016-10-16-NewYork-Philharmonic-Performance/unnamed-chunk-6-1.png)

In the plot above, the dashed lines are the years where there was a tour as 
detailed on their [website](http://nyphil.org/about-us/history/overview).  The 
size of the dots indicate the number of performances in that country that year
while the colours indicate the *total* number of times they have performed in 
that particular country over the entire period.

The colour coding really highlights just how popular Germany is as a destination 
for the group, and more broadly, Europe.  Japan looks like the only main Asian
country that gets a regular NY Phil visit.

What I also found really fascinating was the almost complete absence of any tours
between 1925 and 1950 (Canada would technically be a foreign country but you 
could argue it's just next door).  Where do you first go after a 25 year touring 
hiatus?  Why Scotland, of course.  

The other feather in the diplomatic cap for the group was its 2008 performance 
in North Korea.  It's really quite incredible how widely traveled the group 
has been.


### Composer groupings?
The last thing I will look at for this post will be composer pairings--i.e. 
how many times do combinations of composers occur in the same performance.  The 
heat map below shows the number of times (in log scale) works of both composers are
performed in a performance.  Pairings that don't exists or consist of only a 
single composer have been shaded green.


![center](/images/2016-10-16-NewYork-Philharmonic-Performance/unnamed-chunk-7-1.png)

The plot shows a very high concentration of performances at the bottom left 
corner--which is quite expected given that these are the top most performed 
composers.  

There is quite a dominant American composer themed program with composers like
Gershwin, Copland, Bernstein, Rogers and Sousa all having high counts of 
combinations.  This is most obvious when looking at the row for Richard Rogers
which is mostly blueish (low counts) but *"lights up"* red for columns with 
Gerswhin, Copland, Bernstein and Sousa.  A similar trend is also evident for the 
trio of Russian composers: Rachmaninoff, Shostakovich and Rimski-Korsakov.

Although slightly difficult to see from the heatmap, Beethoven does seem to be
performed with a wider variety of other composers more frequently than Wagner, 
even though Wagner has had more of his works performed.  A closer look at the 
distribution of performances involving these two composers confirms this:

![center](/images/2016-10-16-NewYork-Philharmonic-Performance/unnamed-chunk-8-1.png)

So, although Wagner is played the most often, his works are not performed as 
widely with other composers (at least composers in the top 50).  In contrast,
Beethoven seems to be paired more often with a wider number of other composers.
I do wonder if this has anything to do with how varied Beethoven's work is--from
being more classically bent in his early works to being more romantic at the 
end of his career.  It would be a nice explanation but I will leave that for 
people more knowledgeable than me on that to comment.


### The End
It's always very exciting finding nuggets of information after doing a bit of 
exploratory analysis and this was personally very rewarding.  If anything
it's given me some ideas about my next playlist.  

The R markdown file with the complete code used to generate all these visuals 
can be found here on my [Github repository](https://github.com/jiunsiew/jiunsiew.github.io/tree/master/_R/2016-10-16-NewYork-Philharmonic-Performance.Rmd)

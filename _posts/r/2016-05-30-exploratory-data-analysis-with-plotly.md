---
layout: post
title: "Exploratory Data Analysis with Plotly @ MelbURN"
modified:
categories: r
excerpt: "A presentation on using Plotly to the Melbourne R Users Network."
tags: [R, data visualisation]
comments: true
share: true
image:
  feature:
date: 2016-05-30
---
I recently had the honour of presenting at the Melbourne User of R Network 
(MelbURN) where I presented on the R package `plotly`.

![](/images/2016-05-30-exploratory-data-analysis-with-plotly/highres_450542663.jpeg)

In this presentation, I compared doing some exploratory data analysis with 
[Plotly](https://plot.ly) using the performance data of the New York Philharmonic
Orchestra (which is publically available 
[here](https://github.com/nyphilarchive/PerformanceHistory)).


The presentation can be found here and is best viewed with Chrome or Safari:
[http://jiunsiew.github.io/plotly_presentation/](http://jiunsiew.github.io/plotly_presentation/)

The main point of the talk is really that Plotly plots can be really powerful, 
primarily because of its interactivity capabilities.  

One of my own learnings from using Plotly is that interactive capabilities 
requires a bit of rethink around how to actually visualise data.

A good example is that with interactivity it's quite possible to plot a number 
of time series' on one plot and simply toggle the series on and off rather than 
doing a facet plot, as is common to do with ggplot (see [slide 18](http://jiunsiew.github.io/plotly_presentation/#18)).

It was also my second foray into using the [slidify](http://slidify.org) package.  
Have to say I was pretty impressed.

**Update (2016-06-06)**

I was also very graciously asked to present this to the Melbourne Python Users Group.

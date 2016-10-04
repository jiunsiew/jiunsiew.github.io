---
layout: post
title: "R distribution prefixes - in plain English"
modified:
categories: r
excerpt: "The A-Z of of distribution prefixes in R."
tags: [R, statistics]
comments: true
share: true
image:
  feature:
date: 2015-05-23
---

In R, probability distributions are prefixed by four different letters, namely 
`d, p, q` and `r`. I've always found the help pages on these fairly hard to 
understand mainly because the terminology used is not what I've been 
taught--nothing wrong with that, just makes things a bit confusing. 

If you're familiar with *PDF's* and *CDF's*, then here's the mapping from R help to more common language.

Prefix |   R help descriptor        |   My interpretation
------ | ------------------------   | ---------------------------------------------------
`d`    |   density                  |   Probability Density Function (PDF)
`p`    |   distribution function    |   Cumulative Distribution Function (CDF)
`q`    |   quantile function        |   Inverse CDF
`r`    |   generate random deviates |   Draw random samples from the specified distribution

A really good reference for this is found here:
[http://www.stat.umn.edu/geyer/old/5101/rlook.html](http://www.stat.umn.edu/geyer/old/5101/rlook.html)

Another post that explains PDFs, CDFs and the quantile function is here:
[https://www.countbayesie.com/blog/2015/4/4/parameter-estimation-the-pdf-cdf-and-quantile-function](https://www.countbayesie.com/blog/2015/4/4/parameter-estimation-the-pdf-cdf-and-quantile-function)

#### A simple example
Let's say you have a Chi-squared distribution with one degree of freedom and you 
want to find \\( P(x < 3.84) \\). This is simply:


{% highlight r %}
pchisq(3.84,1)
{% endhighlight %}



{% highlight text %}
## [1] 0.9499565
{% endhighlight %}
And, using the inverse CDF function:


{% highlight r %}
qchisq(0.95,1)
{% endhighlight %}



{% highlight text %}
## [1] 3.841459
{% endhighlight %}


We can use the `d` function to generate a plot of the distribution:


{% highlight r %}
x <- seq(0.02,3,by=0.02)
y <- dchisq(x,1)
plot(x,y, main = 'Chi-squared, DF = 1')
{% endhighlight %}

![center](/images/2015-05-23-R-distribution-prefixes/unnamed-chunk-3-1.png)

Arguably, the easiest prefix to understand is the `r` prefix which simply draws random samples from the specified distribution.


{% highlight r %}
set.seed(1234)
rchisq(5,1)
{% endhighlight %}



{% highlight text %}
## [1] 0.0362440038 2.2213559512 0.0002527833 0.2241010924 0.2296672661
{% endhighlight %}

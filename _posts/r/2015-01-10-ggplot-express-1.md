---
layout: post
title: "ggplot express (1)"
modified:
categories: r
excerpt: "Quick tips in ggplot-ing."
tags: [R, ggplot, data visualisation]
comments: true
share: true
image:
  feature:
date: 2015-01-10

---

Here are some code snippets with ggplot that I use very often but forget equally 
as much.



{% highlight r %}
# to set the theme so you don't have to keep typing "+ theme_bw()"
theme_set(theme_bw())

# to rotate the x-axis labels
theme(axis.text.x = element_text(angle = 90, hjust = 1))

# to put the legend at the bottom/top/wherever
theme(legend.position = 'bottom')  # or wherever you want--see the help

# to stack the legend icons and position the legend title
guides(col = guide_legend(ncol = 7, title.position = 'top'))
{% endhighlight %}
---
layout: post
title: "Comparing models with AIC and standard deviations:  the devil in the details"
modified:
categories: r
excerpt:  
tags: [R, statistics]
comments: true
share: true
image:
  feature:
date: 2014-10-10
---



I've recently had to do some model evaluations where the models had quite different characteristics. 
One model was a straightforward linear regression, the second one was the same but had more samples, 
and the others were using non-linear methods like random forest and gradient boosting machines.

This led to the common conundrum about how to evaluate and compare them. 
I won't go into that here as there are much better sources of information on this topic 
(see for example Chapter 7 of *Hastie, Tibshirani, Friedman's* book 
[Elements of Statistical Learning](http://statweb.stanford.edu/~tibs/ElemStatLearn/) 
and the R [caret](http://cran.r-project.org/web/packages/caret/index.html) package).

Two common measures used to evaluate models are the AIC and the standard deviation of the 
residuals, \\(\sigma\\). In comparing these values, I found a couple of minor details that are 
worth keeping in mind when using these values.

### Calculating AIC
The AIC is defined as \\(AIC = 2k - 2\ln(L)\\), where \\(k\\) is the number of parameters in the 
model, and \\(\ln(L)\\) is the maximised log likelihood.  For linear regression models, the 
maximum log likelihood the maximum log likelihood can be calculated explicitly as 
explained in this [https://stat.ethz.ch/pipermail/r-help/2003-June/035518.html](https://stat.ethz.ch/pipermail/r-help/2003-June/035518.html) 
email thread on the R help from 2003:

```
logLikelihood <- sum(dnorm(y, y.hat, sigma, log = TRUE))
```
Here, \\(y\\) is the response or output variable, \\(\hat{y}\\) is the model estimate and 
\\(\sigma\\) is standard deviation. The formula is effectively calculating the density value 
using \\(\hat{y}\\) as the mean and \\(\sigma\\) as the variance for each element in the response 
vector.

In the email thread, the person who first posed the question had an issue with \\(\sigma\\) 
which was determined from the `summary` function. The reason for this is because there is 
a slight difference in the way \\(\sigma\\) is calculated in the `summary` function and the 
normal way of calculating standard deviations.

### Standard deviations and summary values
I found out the hard way, that the standard deviation from the `summary` function takes 
into account the degrees of freedom (or simplistically, the number of input variables, 
including the intercept) when calculating \\(\sigma\\). In other words, instead of 
multiplying by \\(\frac{1}{n}\\), as is in the textbook formula, the multiplication 
factor is by \\(\frac{1}{n-dof}\\).  The `summary` function is effectively providing an 
unbiased estimate of the standard deviation.

Indeed, the solution to the email thread was to calculate the standard deviation manually 
assuming a zero mean normal distribution.

### In practise
To sum up, when comparing models, R has some very useful summary and AIC functions. 
However, if you're comparing models that have different sample sizes, the AIC won't be 
suitable as it's dependent on the number of samples you're using. It's best to use an 
alternative like the MSE or comparing the model residual's standard deviation. 
If you're comparing the standard deviations, keep in mind that R's summary functions 
account for the number of input variables that your model has (it's 'unbiased'). 
So, if your models have different parameters, it might be best to calculate the 
standard deviation explicitly for fairer model comparison.
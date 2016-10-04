---
layout: post
title: "On Variable Importance with Random Forests"
modified:
categories: r
excerpt: "Deciphering and defining variable importance values in the randomForest package"
tags: [R, random forest]
comments: true
share: true
image:
  feature:
date: 2016-10-04
---

## Introduction
One of the big challenges in predictive modelling is determining which variables 
to use as predictors.  This challenge becomes particularly difficult when 
the number of possible predictors becomes very large, making manual 
exploratory methods or the more "traditional" forward/backward stepwise 
regression models impractical, particularly when interactions between variables
need to be considered.

One very practical and useful by-product of the random forest algorithm (and 
gradient boosting machines) is the `variable importance` which can be calculated 
as part of the model fitting process.  It turns out that this metric comes in 
very handy in many practical settings for whittling a large number of 
possible predictors to a more manageable number.

This post brings together (in a non-exhaustive way) the theory and definitions 
behind the calculation of this metric. I'll be using the [`randomForest`](https://cran.r-project.org/web/packages/randomForest/index.html) 
package in R which is an implementation of Leo Brieman's original algorithm.


<!-- ## Preliminaries 
Before we get into what these importance measures are, let's first define some 
notation and terms.


### Note on terminology
Given a typical linear model expression: 

$$y = \sum_{i = 1}^n \alpha_i x_i$$

the variable \\( y \\) is called a variety of things terms including 
*predicted variable*, *dependent variable* and *target variable*.  Likewise, the 
\\( x_i \\) terms are called the *predictor variables*, *independent variables* and 
*covariates*.  These terms are used interchangeably throughout this post.
-->

## Random forest workings
Before we get into what these importance measures are, let's first briefly recall 
how the random forest (RF) algorithm works.

At the very heart of the random forest algorithm lies the decision trees--a whole
*"forest"* of them in fact.  Each tree in the forest is built using bootstrapped 
samples.

<!--
> Each tree is constructed using a different bootstrap sample from the original 
data. About one-third of the cases are left out of the bootstrap sample and not 
used in the construction of the kth tree. 
-->

The samples that are not included in the bootstrap are called *out of bag* (OOB) 
and can be used to calculate an OOB error estimate.  Averaged over all cases, 
this OOB error is unbiased and is one of the attractive features of RF--it 
effectively does cross validation on the fly.

More detailed descriptions on RF is very easily found; both [1] and [2] are 
good places to start.

<!--
> Put each case left out in the construction of the kth tree down the kth tree 
to get a classification. In this way, a test set classification is obtained for 
each case in about one-third of the trees. At the end of the run, take j to be 
the class that got most of the votes every time case n was oob. The proportion 
of times that j is not equal to the true class of n averaged over all cases is 
the oob error estimate. This has proven to be unbiased in many tests. 

As an aside, this feature of random forest is partly why it has become so 
popular as it circumvents the need for cross validation.
-->

## Model Training with Random Forest

Let's start with a very simple model fit using the ubiquitous `iris` dataset.  Say 
we're given the task of trying to predict `Species` and we're not sure which 
are the best variables to pick (granted, in this case having only 4 independent 
variables is rather trivial but the concepts here should be generalisable).  We 
can use the `randomForest` algorithm with the `importance` input variable set to 
`TRUE`.


{% highlight r %}
library(randomForest)
data("iris")

modelFit <- randomForest(Species ~ ., data = iris, importance = TRUE)
importance(modelFit)
{% endhighlight %}



{% highlight text %}
##                 setosa versicolor virginica MeanDecreaseAccuracy
## Sepal.Length  7.119830  7.5497220  8.265221            11.169925
## Sepal.Width   5.195345  0.9140304  3.999435             4.626126
## Petal.Length 23.936516 35.4103356 27.545759            34.680518
## Petal.Width  20.706746 31.3415373 28.951455            31.634409
##              MeanDecreaseGini
## Sepal.Length        10.928226
## Sepal.Width          2.328845
## Petal.Length        43.223555
## Petal.Width         42.788375
{% endhighlight %}

The importance table has one row for each predictor variable used.  The first
three columns are specific to this dataset and are related to each class of the
predicted variable.  Then, there are two columns which are the main columns of
interest: `MeanDecreaseAccuracy` and `MeanDecreaseGini`.  

Lets go through each of these columns separately.  


### Variable Importance and Mean Decrease Accuracy

The values of the first three columns contain the *raw importance score* for 
each variable in each class.  A description of how this is calculated is found 
in [1]:

> In every tree grown in the forest, put down the oob cases and count the number 
of votes cast for the correct class. Now randomly permute the values of variable 
m in the oob cases and put these cases down the tree. Subtract the number of 
votes for the correct class in the variable-m-permuted oob data from the number 
of votes for the correct class in the untouched oob data. The average of this 
number over all trees in the forest is the raw importance score for variable m.

In other words, for every tree, two sets of predictions are computed and 
compared.  The first prediction is the OOB prediction as described above.  The 
second prediction is generated using a ***random permutation*** of the values in 
each predictor variable.  The raw importance score is the average difference 
between the number of correctly classified samples of these two predictions in 
each class. The final reported scores are normalised by the standard deviation 
of the differences [3].

To get the Mean Decrease Accuracy the raw importance scores are averaged across 
all classes and normalised. 

Two important things to note here about this metric are:

1. It is calculated relative to a random permutation in the OOB samples 
2. It is based on prediction accuracy.


It is also worth noting a couple of things:

1. A key assumption in the calculation of the standard deviation of accuracy 
differences is that the raw importance scores between trees are independent [1]. 
2. For regression, the error metric used is the Mean Square Error.
3. To get the un-normalised scores, set `scale = FALSE` in the `importance` 
function call.



### Mean Decreases Gini

To understand this metric, first let's define the Gini index.  Since RF's are 
based on decision trees, we can define the proportion of class \\( k \\) 
observations in node \\(m\\) by [2, pg 309]:

$$\hat{p}_{mk} = \frac{1}{N_m} \sum_{x_i \in R_m} I\left(y_i = k\right)$$

where \\( R_m \\) is the region represented by node \\( m \\) and \\(N_m\\) is 
the number of observations in node \\(m\\).  

The Gini index is thus defined as:

$$Gini = \sum_{k=1}^K \hat{p}_{mk}(1-\hat{p}_{mk})$$

The equation above simply calculates the ***node impurity*** by splitting at
node \\(m\\).  Put simply, it's a measure of how many classes are found in a 
region after splitting a node.  If for example, the split separates all 
observations into one class, then \\( \hat{p}_{mk} = 1\\).

With random forests, the Gini decrease is measured by comparing the Gini index
before and after the node is split on a particular variable.  This is then summed
over all trees and averaged over variables to get the Mean decrease Gini [1]:

<!--
> Every time a split of a node is made on variable m the gini impurity criterion 
for the two descendent nodes is less than the parent node. Adding up the gini 
decreases for each individual variable over all trees in the forest gives a fast 
variable importance that is often very consistent with the permutation 
importance measure.
-->

Again, for regression, the process is the same except that the Gini index is 
replaced by the residual sum of squares [3].


## Application
How would you use this in real-world settings?  Using the RF model, you could 
simply use all your predictors and rank their importance using either the accuracy 
or node impurity metrics above. Depending on your application, you could then 
look at selected only a subset of variables from your original lists.  You 
should of course do the model refitting with these variable subsets with 
cross-validation (if you're using another algorithm) to get a sense of the 
error rates of your final model.

One limitation to be aware of is the correlation between predictors since the 
variance of RF models with non-independent predictors is lower bounded by the 
positive pairwise correlation (see Equation 15.1 in [2]).  


## TL;DR (also know as a conclusion)
To sum up, you can use the RF model to whittle a large number of predictors 
to a much smaller set with the Mean Decrease Gini or Mean Decrease Accuracy.

The accuracy metric is calculated relative to a random permutation of values in
the OOB samples and normalised by standard deviation of the differences.

The Gini metric is calculated based on the decrease in node impurity by splitting
on a variable.

Once you've selected your important variables, be sure to fit your model again 
using cross validation to get a good sense of how genarlisable your model is.


## References
[1] "Random forests - classification description", *Stat.berkeley.edu, 2016.* [Online]. Available: [https://www.stat.berkeley.edu/~breiman/RandomForests/cc_home.htm](https://www.stat.berkeley.edu/~breiman/RandomForests/cc_home.htm). [Accessed: 03- Oct- 2016].

[2] T. Hastie, R. Tibshirani and J. Friedman, *The elements of statistical learning.*, 2nd ed.

[3] A. Liaw and M. Wiener, *Package 'randomForest'; Breiman and Cutler's Random Forest for Classification and Regression*, v4.6-12, 2015-10-06. Available: [https://cran.r-project.org/web/packages/randomForest/randomForest.pdf](https://cran.r-project.org/web/packages/randomForest/randomForest.pdf)


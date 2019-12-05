

#  PA3 - Naive Bayes Text Classifier!


## HOW TO EXECUTE

1. Compile libstemmer.c for using text stemmer in libstemmer_c folder.

``` $ cd libstemmer_c ```  
``` $ make ```

2. You have to move libstemmer.o into lib folder.

``` $ mv libstemmer.o ../lib ```

3. Compile trainer and predictor.

In code folder,

``` $ make ```

4. First you have to execute trainer to train data.

```  $ ./trainer ```


5. And execute predictor to check whether the sentence you entered is negative or not.
 
``` $ ./predictor ```

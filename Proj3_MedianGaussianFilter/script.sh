#!/bin/bash
java -jar ./LohH_Project3_Java.zip MedianGaussFilters_data.txt MedianGaussFilters_Template.txt MedianResult.txt GaussResult.txt


echo Median Filter
echo ----------------------------------------

mkdir MedianFilter

./histogram.exe MedianResult.txt MedianHist.txt

./bimean.exe MedianHist.txt histImg.txt gaussImg.txt gapImg.txt MedianThr.txt

line=$(head -n 1 MedianThr.txt)

echo $line

./threshold.exe MedianResult.txt MedianFilterBinary.txt prettyMedianFilterBinary.txt
echo generated MedianFilter

mv MedianHist.txt ./MedianFilter
mv histImg.txt ./MedianFilter
mv gaussImg.txt ./MedianFilter
mv gapImg.txt ./MedianFilter
mv MedianThr.txt ./MedianFilter
mv MedianFilterBinary.txt ./MedianFilter
mv prettyMedianFilterBinary.txt ./MedianFilter


echo -e "\n"
echo Gaussian Filter
echo ----------------------------------------

mkdir GaussFilter

./histogram.exe GaussResult.txt GaussHist.txt

./bimean.exe GaussHist.txt histImg.txt gaussImg.txt gapImg.txt GaussThr.txt

line=$(head -n 1 GaussThr.txt)

echo $line

./threshold.exe GaussResult.txt GaussFilterBinary.txt prettyGaussFilterBinary.txt
echo generated GaussFilter

mv GaussHist.txt ./GaussFilter
mv histImg.txt ./GaussFilter
mv gaussImg.txt ./GaussFilter
mv gapImg.txt ./GaussFilter
mv GaussThr.txt ./GaussFilter
mv GaussFilterBinary.txt ./GaussFilter
mv prettyGaussFilterBinary.txt ./GaussFilter


echo -e "\n"
echo nonFilter
echo ----------------------------------------

mkdir nonFilter

./histogram.exe MedianGaussFilters_data.txt nonFilterHist.txt

./bimean.exe nonFilterHist.txt histImg.txt gaussImg.txt gapImg.txt nonFilterThr.txt

line=$(head -n 1 nonFilterThr.txt)

echo $line

./threshold.exe MedianGaussFilters_data.txt nonFilterBinary.txt prettyNonFilterBinary.txt
echo generated nonFilter

mv nonFilterHist.txt ./nonFilter
mv histImg.txt ./nonFilter
mv gaussImg.txt ./nonFilter
mv gapImg.txt ./nonFilter
mv nonFilterThr.txt ./nonFilter
mv nonFilterBinary.txt ./nonFilter
mv prettyNonFilterBinary.txt ./nonFilter

read "Press Return to Close..."
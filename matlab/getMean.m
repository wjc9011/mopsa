% get mean x,y,xya
function res = getMean(nodeSet, p)
total = numel(nodeSet);

if total == 1
    pMean = p(nodeSet);
else
    i = 1;
    pTotal = 0;
    while i <= total
        pTotal = pTotal + p(nodeSet(i));
        i = i + 1;
    end
    pMean = pTotal / total;
end
res = pMean;
    
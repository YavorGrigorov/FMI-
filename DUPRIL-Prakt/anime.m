function anime
nrf=100;
M=moviein(nrf)
for i=1:nrf/2
    fimplicit(@(x,y) x.^3 + y.^3 - x.*y.*3 - (nrf/2-i)/100, [-2 2 -2 2])
    hold on
    M(: ,i) = getframe
end
for i=nrf/2:nrf
    fimplicit(@(x,y) x.^3 + y.^3 - x.*y.*3 + (i-nrf/2)/100, [-2 2 -2 2])
    M(:, i) = getframe
end
end
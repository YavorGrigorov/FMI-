function graph3D
x=-10:0.1:10;
y=-10:0.1:10;
[X,Y]=meshgrid(x,y);
%Z1=X.*Y./(X.^2+Y.^2);
Z2=(X.^3)+(Y.^3)-(X.*Y.*3);
surfl(X,Y,Z2)
end


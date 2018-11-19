%   y'(x) = f(x, y)
%   y(x0) = y0
%   yk(x) = y0 + S[x0, x](f(t, yk-1(t))dt   Метод на Пикар (за решаване /\)
%
%   Да се реши символно зад на Коши  y' = y, y(0) = 1
%       и да се начертае с червен цвят графиката на решението в [-4, 4]
%   Да се начертаят с черен цвят графиките на първите пет приближеня на
%       решението получено по метода на Пикар
%

function Pikar
    hold on
    grid on
    
    xmin = -10; xmax = 10;
    x0 = 0; y0 = 1;
    
    axis([xmin xmax -10 20]);
    y = dsolve('Dy=y', 'y(0)=1');
    t = xmin:(xmax-xmin)/100:xmax;
    
    plot(t, eval(y), 'r')
    xlabel('x'); ylabel('y');
    
    N = 5;
    x = x0:(xmin - x0)/100:xmin;
    xx = x0:(xmax-x)/100:xmax;
    
    y_0 = y0*ones(1, length(x));
    yy_0 = y0*ones(1, length(xx));
    
    z = y_0; zz = yy_0;
    for k=1:N
        y_k = y0 + cumtrapz(x, ff(x, z));
        yy_k = y0 + cumtrapz(xx, ff(xx, zz));
        plot(x, z, 'k', xx, zz, 'k')
        
        z = y_k; zz = yy_k;
    end
    
    function z=ff(x,y)
        z = y
    end
end

% да се начертае полето от прави на а) y' = x ^ 2 + y ^ 2
%                                   б) yy' = x\
% в квадрата К = [-5, 5] x [-5, 5]
%   В същия прозорец, да се начертае графиката на решението на зад. на Коши
%   за даденото у-ние с начална цел у(х0) = у0, където т.(х0, у0) се
%   въвежда с клик в/у К.
%
% у' = f(x, y)
%   Полето от прави е :
% { (x,y) от R^2 | y - y0 = f(x0, y0)*(x - x0), (x0, y0) е от D}
% представляват всички допирателни (?)
%
function Plotslope
    x= -5:0.3:5;
    y= -5:0.3:5;
    
    delta = 0.2; % ?
    hold on
    axis([-6 6 -6 6])
    grid on
    
    for k=1:length(x)
        for m=1:length(y)
            eps = delta / (sqrt(1 + ff(x(k), y(m)^2))); % за да е както трябва да е дълга отсечката
            plot([x(k) - eps, x(k) + eps], [y(m) - eps*ff(x(k), y(m)), y(m) + eps*ff(x(k), y(m))], 'k')
        end
    end
    
    % черате графиката като кликнеш
    while true
    [x0, y0] = ginput(1);
    plot(x0, y0, 'bo')
    
    [T, Y]=ode45(@ff, [x0, 5], y0)
    [T1, Y1]=ode45(@ff, [x0, -5], y0)
    plot(T, Y, 'b', T1, Y1, 'b')
    end
    function z=ff(x, y)
        %z = x^2 + y^2;
        z = x/y;
    end
end



%   y - y0 = f(x0, y0) (x - x0)
%   param:
%           x = x0 + t / (sqrt(1 + f^f(x0, y0)))
%           y = y0 - x0*f(x0, y0) - t*f(x0, y0) / (sqrt(1 + f^2(x0,y0)))
%
%




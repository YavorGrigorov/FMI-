% �� �� ������� ������������ � ��������� �-�� �� �-�����:
%       �) (y')^2 -xy' + 5y - x^2 = 0
%       �) y = xy' +(y')^2
%   � �� �� �������� ��������� �� ��������� �� �������.
%   � ������ �������� � �������� ����, �� �� ��������� ��������� ��
%   ��������� �������, �������� ���� ���������� ����� (x0, y0), ��������
%   ���� ���� � [-8, 8]x[-8, 8].
%   ��� (�0, �0) �� � ���� ������� ���� ����������, �� �� ������ ���������,
%   �� ���� ��� �� ������ ������� �������.
%
%   (x0, y0) �� ������ ���������� �� F(x, y, y') , ��� �� ������ z, ��
%   ����� F(x0, y0, z) = 0, (dF/dz)(x0, y0, z) != 0 � ���������
%   ��� (dF/dz)(x0, y0, z) = 0 -> � �������
%
%   y = x*z + z^2 //diff
%   | dy = x*dz +z*dx + 2z*dz
%   | dy = zdx
%   => z*dx = x*dz + z*dx + 2z*dz -> x*dz + 2z*dz = 0
%   -> (x + 2z)*dz = 0 <=> dz = 0 && x + 2z = 0
%   <=> dz = 0 && x = -2z
%   => 1 ��. y = -2zz + z^2 -> y = -z^2  
%   => 2 ��. dz = 0 => z = c //const => y = cx + c^2
%


function Unsolved

% setup of screen
    axis([-10 10 -10 10])
    hold on
    grid on
%
    syms x y z
    %ff = z^2 + 5*y - x^2 - x*z; % a)
    ff = x*z + z^2 - y; % b)
    dff = diff(ff,z);
    
    [y, z] = solve(ff, dff, y, z)
    x = -10:1/100:10;
    yy = eval(y);
    
    plot(x, yy, 'k')
    
    while true
    [x0, y0] = ginput(1);
    plot(x0, y0, 'r*')
    if y0 < - (x0^2)/4
        text(x0 + 0.1, y0, 'mo solution')
    else
        %y1 = dsolve('(Dy)^2 - x*Dy - x^2 + 5*y = 0', 'y(x0) = y0', 'x') % a)
        y1 = dsolve('(Dy)^2 + x*Dy - y = 0', 'y(x0) = y0', 'x') % b)
        plot(x, eval(y1))
    end
    end
end





function [angle1, angle2, angle3, angle4] = InverseKinematics(A1, A2, A3, A4, x, y, z)
    j1 = atan2(y,x);
    len = sqrt(y*y+x*x);
    high = z;
    for alpha = -pi:0.01:pi
        L = len-A4*sin(alpha);
        H = high-A1-A4*cos(alpha);
        c3 = (L*L+H*H-A2*A2-A3*A3)/(2*A2*A3);
        if c3*c3 < 1
            s3 = sqrt(1-c3*c3);
            j3 = atan2(s3, c3);
            K1 = A2 + A3*cos(j3);
            K2 = A3*sin(j3);
            w = atan2(K2, K1);
            j2 = atan2(L, H)-w;
            j4 = alpha - j2 - j3;
        end
    end
    angle1 = j1/pi*180;
    angle2 = 90-j2/pi*180;
    angle3 = 90-j3/pi*180;
    angle4 = 90-j4/pi*180;
end
function angle = LinearInterpolation(startAngle, targetAngle, stepAngle)
    if (targetAngle - startAngle) > stepAngle
        angle = startAngle + stepAngle;
    elseif (targetAngle - startAngle) < -stepAngle
        angle = startAngle - stepAngle;
    else
        angle = targetAngle;
    end
end
function angle = limit(targetAngle)
    if targetAngle <= -180
        angle = targetAngle + 360;
    elseif targetAngle >= 180
        angle = targetAngle - 360;
    else 
        angle = targetAngle;
    end
end
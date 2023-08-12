int angleDiff(int Setpoint, int CurrentAngle) { 
	int diff = Setpoint - CurrentAngle;
	if (diff > 180) {
		diff -= 360;
	} else if (diff < -180) {
		diff += 360;
	} 
	return diff;
}

using System;
using Godot;

public static class Tools
{
	/// <summary>
	/// Gets a random number between the min and max provided.
	/// </summary>
	public static float RandRange(float min = 0, float max = 1)
	{
		return float.Lerp(min, max, GD.Randf());
	}

	/// <summary>
	/// Reduces a value by the specified deceleration amount, without going beneath 0.
	/// </summary>
	public static float Decelerate(float org, float decel, float min_speed = 0)
	{
		int sign = MathF.Sign(org);
		float abs_org = MathF.Abs(org);
		abs_org -= MathF.Abs(decel);
		if (abs_org <= min_speed) abs_org = min_speed;
		return abs_org * sign;
	}

	/// <summary>
	/// Increases a value by the specified acceleration amount, without going above a max_speed. Allows accelerating in a negative direction.
	/// </summary>
	public static float Accelerate(float org, float accel, float max_speed)
	{
		if (MathF.Abs(org) < max_speed || MathF.Abs(org + accel) < MathF.Abs(org)) org += accel;
		return org;
	}

	/// <summary>
	/// Decelerates a vector's magnitude while keeping the same direction.
	/// </summary>
	public static Vector3 Decelerate(Vector3 org, float decel)
	{
		if (org.Length() == 0) return org;
		Vector3 org_dir = org.Normalized();
		float magnitude = org.Length();
		return org_dir * Decelerate(magnitude, decel);
	}

	/// <summary>
	/// Decelerates a vector's magnitude while keeping the same direction. Without altering the Y value. Used for friction that does not affect gravity.
	/// </summary>
	public static Vector3 DecelerateFlat(Vector3 org, float decel)
	{
		Vector3 fix_dir = Decelerate(org, decel);
		fix_dir.Y = org.Y;
		return fix_dir;
	}

	/// <summary>
	/// Decelerates a vector by a specified deceleration in each axis.
	/// </summary>
	public static Vector3 Decelerate(Vector3 org, Vector3 decel)
	{
		return new Vector3(Decelerate(org.X, decel.X), Decelerate(org.Y, decel.Y), Decelerate(org.Z, decel.Z));
	}

	/// <summary>
	/// Accelerates a vector by a specified acceleration value without going above a maximum speed. Allows accelerating in a negative direction.
	/// </summary>
	public static Vector3 Accelerate(Vector3 org, Vector3 accel, float max_speed)
	{
		Vector3 new_vec = org + accel;
		if (new_vec.Length() < org.Length()) return new_vec; // Results in a slower vec, allow it
		if (new_vec.Length() < max_speed) return new_vec; // Less than max, allow it
		return org; // At limit
	}

	/// <summary>
	/// Step toward a specified goal value from the current value, at a fixed rate.
	/// </summary>
	public static float StepToward(float current, float goal, float rate)
	{
		if(rate == float.PositiveInfinity) return goal;
		if(Mathf.Abs(current - goal) <= rate) return goal;
		if(current < goal) return Mathf.Min(goal, current + rate);
		if(current > goal) return Mathf.Max(current - rate, goal);
		return goal;
	}
	
	/// <summary>
	/// Step toward a specified goal value from the current value, at a fixed rate.
	/// </summary>
	public static Vector2 StepToward(Vector2 current, Vector2 goal, float rate)
	{
        return new()
        {
            X = StepToward(current.X, goal.X, rate),
            Y = StepToward(current.Y, goal.Y, rate)
        };
	}

	/// <summary>
	/// Step toward a specified goal value from the current value, at a fixed rate.
	/// </summary>
	public static Vector3 StepToward(Vector3 current, Vector3 goal, float rate)
	{
        return new()
        {
            X = StepToward(current.X, goal.X, rate),
            Y = StepToward(current.Y, goal.Y, rate),
            Z = StepToward(current.Z, goal.Z, rate)
        };
	}
	
	/// <summary>
	/// Step toward a specified goal value from the current value, at a fixed rate.
	/// </summary>
	public static Color StepToward(Color current, Color goal, float rate)
	{
        return new()
        {
            R = StepToward(current.R, goal.R, rate),
            G = StepToward(current.G, goal.G, rate),
            B = StepToward(current.B, goal.B, rate),
            A = StepToward(current.A, goal.A, rate)
        };
	}

	/// <summary>
	/// Gets the vector from one point to another at full length without normalization
	/// </summary>
	public static Vector3 VectorTo(Vector3 start, Vector3 end)
	{
		if(start == end) return Vector3.Zero;
		return end - start;
	}

	/// <summary>
	/// Gets a normalized vector pointing from the start location to the end location.
	/// </summary>
	public static Vector3 DirVector(Vector3 start, Vector3 end)
	{
		return VectorTo(start,end).Normalized();
	}
	
	/// <summary>
	/// Gets a normalized direction vector with the Y value flattened to zero.
	public static Vector3 FlatDirVector(Vector3 dir)
	{
		dir.Y = 0;
		return DirVector(Vector3.Zero, dir);
	}

	/// <summary>
	/// Gets a normalized direction vector with the Y value flattened to zero.
	public static Vector3 FlatDirVector(Vector3 start, Vector3 end)
	{
		start.Y = 0;
		end.Y = 0;
		return DirVector(start, end);
	}

	/// <summary>
	/// Gets the distance between two 3D points, but with their y values both set to zero.
	/// </summary>
	public static float FlatDistance(Vector3 start, Vector3 end)
	{
		start.Y = 0;
		end.Y = 0;
		return start.DistanceTo(end);
	}

	/// <summary>
	/// Get the eular angles of a direction vector, as if it was a camera rotated to face that direction.
	/// </summary>
	public static Vector3 GetDirEulars(Vector3 dirvec)
	{
		return new Transform3D().LookingAt(dirvec).Basis.GetEuler();
	}
	
	/// <summary>
	/// Get the eular angles of a direction vector, as if it was a camera rotated to face that direction.
	/// </summary>
	public static Vector3 GetDirEulars(Vector3 start, Vector3 end)
	{
		return GetDirEulars(DirVector(start, end));
	}

	/// <summary>
	/// Get the eular angles of a direction vector, as if it was a camera rotated to face that direction.
	/// </summary>
	public static Vector3 GetFlatDirEulars(Vector3 dirvec)
	{
		dirvec.Y = 0;
		return new Transform3D().LookingAt(dirvec).Basis.GetEuler();
	}

	/// <summary>
	/// Get the eular angles of a direction vector, as if it was a camera rotated to face that direction.
	/// </summary>
	public static Vector3 GetFlatDirEulars(Vector3 start, Vector3 end)
	{
		start.Y = 0;
		end.Y = 0;
		return GetDirEulars(DirVector(start, end));
	}

	/// <summary>
	/// Get the rotation quaternion of a direction vector, as if it was a camera rotated to face that direction.
	/// </summary>
	public static Quaternion GetDirQuaternion(Vector3 dirvec)
	{
		return new Transform3D().LookingAt(dirvec).Basis.GetRotationQuaternion();
	}
	
	/// <summary>
	/// Get the rotation quaternion of a direction vector, as if it was a camera rotated to face that direction.
	/// </summary>
	public static Quaternion GetDirQuaternion(Vector3 start, Vector3 end)
	{
		return GetDirQuaternion(DirVector(start, end));
	}
}
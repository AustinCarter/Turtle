
--[[
Called once a frame
Params: Entity: the entity this script belongs to
		ts: The time (in seconds) since the last update
--]]
function OnUpdate(Entity, ts)
	local transform = Entity:Get__TransformComponent() 
	transform.Rotation.z = transform.Rotation.z +  5 * ts
end
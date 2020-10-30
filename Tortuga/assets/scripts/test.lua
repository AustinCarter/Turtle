--[[
	Properties that will be exposed to c++
--]]
Props = {
	TestVal = 10,
	CoolerVal = 1
}

--[[
Called once a frame
Params: Entity: the entity this script belongs to
		ts: The time (in seconds) since the last update
--]]
function OnUpdate(Entity, ts)
	local transform = Entity:Get__TransformComponent() 
	transform.Translation.x = math.sin(transform.Rotation.z)
	transform.Translation.y = math.cos(transform.Rotation.z)
	transform.Rotation.z = transform.Rotation.z +  3 * ts
	local sprite = Entity:Get__SpriteRendererComponent()
	sprite.Color.x = (math.sin(transform.Rotation.z)/2) + .5
	sprite.Color.z = (math.cos(transform.Rotation.z)/2) + .5
end
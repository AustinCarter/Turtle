--[[
	Properties that will be exposed to c++
--]]
Props = {
	Speed = 0.5
}

--[[
Called once a frame
Params: Entity: the entity this script belongs to
		ts: The time (in seconds) since the last update
--]]
-- function OnUpdate(Entity, ts)
-- 	local transform = Entity:Get__TransformComponent() 
-- 	transform.Translation.x = math.sin(transform.Rotation.z) + Props.XOffset
-- 	transform.Translation.y = math.cos(transform.Rotation.z) + Props.YOffset
-- 	transform.Rotation.z = transform.Rotation.z +  3 * ts
-- 	local sprite = Entity:Get__SpriteRendererComponent()
-- 	sprite.Color.x = (math.sin(transform.Rotation.z)/2) + .5 
-- 	sprite.Color.z = (math.cos(transform.Rotation.z)/2) + .5 

-- end

--[[
--]]
function OnKeyPressed(Entity, ts)
	local transform = Entity:Get__TransformComponent() 
	transform.Translation.x = math.sin(transform.Rotation.z) + Props.XOffset
	transform.Translation.y = math.cos(transform.Rotation.z) + Props.YOffset
	transform.Rotation.z = transform.Rotation.z +  3 * ts
	local sprite = Entity:Get__SpriteRendererComponent()
	sprite.Color.x = (math.sin(transform.Rotation.z)/2) + .5 
	sprite.Color.z = (math.cos(transform.Rotation.z)/2) + .5 
	
end
require("KeyCodes")
--[[
	Properties that will be exposed to c++
--]]
Props = {
	Speed = 0.05
}

--[[
Called once a frame
Params: Entity: the entity this script belongs to
		ts: The time (in seconds) since the last update
--]]
function OnUpdate(Entity, ts)

	local transform = Entity:Get__TransformComponent()

	if(Input.IsKeyPressed(TURT_KEY_D) == 1) then
		transform.Translation.x = transform.Translation.x + Props.Speed
	elseif(Input.IsKeyPressed(TURT_KEY_A) == 1) then
		transform.Translation.x = transform.Translation.x - Props.Speed
	end

	if(Input.IsKeyPressed(TURT_KEY_W) == 1) then
		transform.Translation.y = transform.Translation.y + Props.Speed
	elseif(Input.IsKeyPressed(TURT_KEY_S) == 1) then 
		transform.Translation.y = transform.Translation.y - Props.Speed
	end
end
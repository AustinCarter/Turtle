
-- Called once a frame
function OnUpdate(Entity)
	local transform = Entity:Get__TransformComponent() 
	transform.Translation.x = transform.Translation.x + .01
end

function OnCreate(Entity)
	--print("OnCeate")
end


-- function TestGet(Entity)
-- 	local particles = Entity:Get__ParticleSpawnerComponent() 
-- 
-- end
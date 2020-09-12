# Resources
## TextureID: Gồm 5 chữ số: ABCDE
A : loại object
	A = 1: player
		BC : player type (có thể nhiều loại khí cầu)
		DE : player animation (sử dụng spritesheet)
A = 2 - obstacle
	BC : obstacle type
		BC = 01: starting ground
		BC = 02: small piece
		BC = 03: large piece
		BC = 04: tiny island/cliff
		BC = 05: small island/cliff
		BC = 06: medium island/cliff
		BC = 07: large island/cliff
		BC = 08: giant island/cliff
		BC = 09: titan island/cliff
	DE : Các hình khác nhau của cùng 1 loại obstacle. Animation nếu có
A = 3 - enemy
	BC: enemy type
		BC = 01: Floating fish
		BC = 02: Suicide Bug
		BC = 03: Rocky golem
		BC = 04: Falling rock
		BC = 05: Deadly bird
		BC = 06: Vampire bat
		BC = 07: Witch
		BC = 08: Magic Fireball
	DE: các biến thể của enemy, animation
A = 4 - item
	BC: item type
		BC = 01: Item HP / Healing
		BC = 02: Buff damage
		BC = 03: Buff speed
		BC = 04: Bullet crate (thùng đạn)
	DE: các loại animation, hình trên mỗi thùng đạn khác nhau...
A = 5 - Weapon & bullet
	BC = 1..49: weapon type
		BC = 01: AK
		BC = 02: Cannon
		BC = 03: Pistol
		BC = 04: rifle
		BC = 05: shotgun
		BC = 06: sniper
		BC = 07: uzi
	DE: các loại súng cụ thể trong mỗi mục lớn ở trên. VD: AK47,...
	BC = 51..99: bullet type
		BC = 01: gun bullet
		BC = 02: cannon ball
	DE: đạn có màu sắc, hình dáng khác nhau, animation nếu có
A = 6 - UI
	BC = 01: Health bar
	Bc = 02: Target
	BC = 03: Button
A = 7 - Other

# Misson
ID 0: pass the level
ID 1: Destroy n monsters of type X
ID 2: no blood loss
ID 3: Achieve a minimum of X points
ID 4: No collision into rock

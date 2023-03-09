# Application Structure

> ## Game
> 
> * `std`::`vector`<`Tile`> `Tiles`
> * `std`::`vector`<`Entity`> `Entities`
> * `std`::`vector`<`Irem`> `Items`

> ## Tile
>
> * `bool` `isObstacle` - can entity move through this tile.
> * `string` `charVisible` - hero has LOS on this tile.
> * `string` `charDiscovered` - hero had LOS on this tile and now hasn't.
> * `string` `charUndiscovered` - hero never had LOS on this tile.

> ## Entity
>
> * `bool` `isHostile`
> * `int` `AIID` - id of this entity logic. 
> * `string` `charVisible` - hero has LOS on this entity.
> * `string` `charDiscovered` - hero had LOS on this eintity and now hasn't.
> * `string` `charUndiscovered` - hero never had LOS on this entity.
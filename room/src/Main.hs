import Graphics.UI.WX hiding (Object, Color)
import Prelude hiding ((*>), (<*>))
import Data.Maybe

type Point2D = (Int, Int)

type Point3D = (Double, Double, Double)

type Vec = (Double, Double, Double)

data Ray = Ray Point3D Vec

data Object = Sphere Double Point3D
			| Plane (Double, Double, Double, Double)

type Resolution = (Int, Int)

type Dimension = (Int, Int)

(<+>) :: (Double, Double, Double) -> (Double, Double, Double) -> (Double, Double, Double)
(x1, y1, z1) <+> (x2, y2, z2) = (x1+x2, y1+y2, z1+z2)

(<->) :: (Double, Double, Double) -> (Double, Double, Double) -> (Double, Double, Double)
(x1, y1, z1) <-> (x2, y2, z2) = (x1-x2, y1-y2, z1-z2)

(<*>) :: (Double, Double, Double) -> (Double, Double, Double) -> (Double, Double, Double)
(x1, y1, z1) <*> (x2, y2, z2) = (x1*x2, y1*y2, z1*z2)

(*>) :: (Double, Double, Double) -> Double -> (Double, Double, Double)
(x1, y1, z1) *> f = (x1*f, y1*f, z1*f)

maxF :: Double -> (Double, Double, Double) -> (Double, Double, Double)
maxF f (x, y, z) = (max f x, max f y, max f z)

minF :: Double -> (Double, Double, Double) -> (Double, Double, Double)
minF f (x, y, z) = (min f x, min f y, min f z)

(*.) :: Vec -> Vec -> Double
(x1, y1, z1) *. (x2, y2, z2) = x1*x2 + y1*y2 + z1*z2

len :: Vec -> Double
len v = sqrt (v *. v)

norm :: Vec -> Vec
norm v
	| len v < 10**(-9) = (0.0, 0.0, 0.0)
	| otherwise = v *> (1 / (len v))

mkNormVect :: Point3D -> Point3D -> Vec
mkNormVect a b = norm (a <-> b)

dist :: Point3D -> Point3D -> Double
dist a b = sqrt ((b <-> a) *. (b <-> a))

clip :: (Double, Double, Double) -> (Double, Double, Double)
clip = (maxF 0.0) . (minF 1.0)

solveQuadro :: (Double, Double, Double) -> [Double]
solveQuadro (a, b, c)
	| d < 0 = []
	| d > 0 = [(-b - sqrt d) / (2 * a), (-b + sqrt d) / (2 * a)]
	| d == 0 = [-b / (2 * a)]
	where
		d = b*b - 4*a*c

mkRay :: Point3D -> Point3D -> Ray
mkRay a b = Ray a (mkNormVect a b)

intRayWith :: Ray -> Object -> [Double]
intRayWith (Ray start dir) (Sphere rad cen) = solveQuadro (dir *. dir, 2 * (dir *. d), (d *. d) - rad^2)
	where
		d = start <-> cen
intRayWith (Ray start dir) (Plane (a, b, c, d)) = if (abs (part) < 10**(-9)) then [] else [- (d + ((a, b, c) *. start)) / part]
	where
		part = (a, b, c) *. dir

normal :: Point3D -> Object -> Vec
normal p (Sphere rad cen) = norm ((p <-> cen) *> (1 / rad))
normal _ (Plane (a, b, c, d)) = norm (a, b, c)

reflectDir :: Vec -> Vec -> Vec
reflectDir i n = i <-> (n *> (2 * (n *. i)))

refractDir :: Vec -> Vec -> Double -> Vec
refractDir i n r = if (v < 0) then (0.0, 0.0, 0.0) else norm $ (i *> r_c) <+> (n *> (r_c * (abs c) - (sqrt v)))
	where
		c = n *. (i *> (-1))
		r_c = if (c < 0) then r else (1 / r)
		v = 1 + (r_c ^ 2) * (c ^ 2 - 1)

mapToWin :: Resolution -> Dimension -> Point2D -> Point3D
mapToWin (rx, ry) (w, h) (px, py) = (x / rxD, y / ryD, 0.0)
	where
		(rxD, ryD) = (fromIntegral rx, fromIntegral ry)
		(pxD, pyD) = (fromIntegral px, fromIntegral py)
		(wD, hD) = (fromIntegral w, fromIntegral h)
		(x, y) = ((pxD - rxD / 2) * wD, (pyD - ryD / 2) * hD)

-- Engine

type Color = (Double, Double, Double)

data Diff = Solid Color
	| Perlin (Point3D -> Color)

data Texture = Texture Diff Double Int Double Double

type TextureObject = (Object, Texture)

type Intensity = (Double, Double, Double)

data Light = PointLight Point3D Intensity
	| AmbientLight Intensity

data Camera = Camera Point3D Dimension

data Scene = Scene Camera Color [TextureObject] [Light]

data Intersection = Intersection Double Ray TextureObject

type Image = Point2D -> Color

intDist :: (Maybe Intersection) -> Double
intDist Nothing = 0.0
intDist (Just (Intersection d _ _)) = d

intText :: (Maybe Intersection) -> Texture
intText Nothing = Texture (Solid (0.0, 0.0, 0.0)) 0.0 0 0.0 0.0
intText (Just (Intersection _ _ (_, t))) = t

intPt :: (Maybe Intersection) -> Point3D
intPt Nothing = (0.0, 0.0, 0.0)
intPt (Just (Intersection d (Ray start dir) _)) = start <+> (dir *> d)

colorAt :: (Maybe Intersection) -> Color
colorAt Nothing = (0.0, 0.0, 0.0)
colorAt (Just (Intersection _ _ (_, Texture (Solid color) _ _ _ _))) = color
colorAt i@(Just (Intersection _ _ (_, Texture (Perlin f) _ _ _ _))) = f (intPt i)

normalAt :: (Maybe Intersection) -> Vec
normalAt Nothing = (0.0, 0.0, 0.0)
normalAt i@(Just (Intersection _ _ (o, _))) = normal (intPt i) o

fstPos :: [Double] -> Double
fstPos [] = 0.0
fstPos (l:ls) = if l > 10**(-6) then l else fstPos ls

closestInt :: Ray -> (Maybe Intersection) -> TextureObject -> (Maybe Intersection)
closestInt r i (o, m) = if d > 10**(-6) && ((isNothing i) || d < (intDist i))
	then Just (Intersection d r (o, m))
	else i
	where
		d = fstPos (intRayWith r o)

intersect :: Ray -> [TextureObject] -> (Maybe Intersection)
intersect r o = foldl (closestInt r) Nothing o

diff :: (Maybe Intersection) -> Light -> Color
diff _ (AmbientLight _) = (0.0, 0.0, 0.0)
diff i (PointLight pos int) = (int *> ((mkNormVect (intPt i) pos) *. (normalAt i))) <*> (colorAt i)

spec :: (Maybe Intersection) -> Vec -> Light -> Color
spec _ _ (AmbientLight _) = (0.0, 0.0, 0.0)
spec i d (PointLight pos int) = int *> (reflCoef * (((normalAt i) *. h) ** (fromIntegral specCoef)))
	where
		h = norm ((d *> (-1)) <+> (mkNormVect (intPt i) pos))
		(Texture _ reflCoef specCoef _ _) = intText i

shadePt :: Intersection -> Vec -> [TextureObject] -> Light -> Color
shadePt i d o (AmbientLight int) = int
shadePt i d o l@(PointLight pos int)
	| s = (0.0, 0.0, 0.0)
	| otherwise = (diff (Just i) l) <+> (spec (Just i) d l)
	where
		s = not (isNothing i_s) && (intDist i_s) <= dist (intPt (Just i)) pos
		i_s = intersect (mkRay (intPt (Just i)) pos) o

reflectPt :: Int -> Intersection -> Vec -> [TextureObject] -> [Light] -> Color
reflectPt depth i d = colorPt depth (Ray (intPt (Just i)) (reflectDir d (normalAt (Just i)))) (0.0, 0.0, 0.0)

refractPt :: Int -> Intersection -> Vec -> Color -> [TextureObject] -> [Light] -> Color
refractPt depth i d b = if refractedDir == (0.0, 0.0, 0.0) then (\x y -> (0.0, 0.0, 0.0))
	else colorPt depth (Ray (intPt (Just i)) refractedDir) (b *> refrCoef)
	where
		refractedDir = refractDir d (normalAt (Just i)) refrIndex
		(Texture _ _ _ refrCoef refrIndex) = intText (Just i)

colorPt :: Int -> Ray -> Color -> [TextureObject] -> [Light] -> Color
colorPt (-1) _ _ _ _ = (0.0, 0.0, 0.0)
colorPt d r@(Ray _ dir) b o l = if (isNothing i) then b else clip $ shadeColor <+> reflectColor <+> refractColor
	where
		shadeColor = foldl (<+>) (0.0, 0.0, 0.0) (map (shadePt (fromJust i) dir o) l)
		reflectColor = if (reflCoef == 0.0) then (0.0, 0.0, 0.0)
			else (reflectPt (d - 1) (fromJust i) dir o l) *> reflCoef
		refractColor = if (refrCoef == 0.0) then (0.0, 0.0, 0.0)
			else (refractPt (d - 1) (fromJust i) dir b o l) *> refrCoef
		i = intersect r o
		(Texture _  reflCoef _ refrCoef _) = intText i

rayTracePt :: Int -> Scene -> Point3D -> Color
rayTracePt d (Scene (Camera eye _) b o l) p = colorPt d (Ray p (mkNormVect eye p)) b o l

rayTrace :: Int -> Resolution -> Scene -> Image
rayTrace d r s@(Scene (Camera _ dim) _ _ _) = (rayTracePt d s) . (mapToWin r dim)

--main :: IO ()
--main = start gui

radius, maxX, maxY :: Int
maxY = 300
maxX = 300
radius = 10

-- the max. height is at most max. y minus the radius of a ball.
maxH :: Int
maxH = maxY - radius

--the main function
main = start ballsFrame

ballsFrame
  = do -- a list of balls, where each ball is represented
       -- by a list of all future positions.
       vballs <- varCreate []

       -- create a non-user-resizable top-level (orphan) frame.
       f <- frameFixed [text := "Bouncing balls"]

       -- create a panel to draw in.
       p <- panel f [on paint := paintBalls vballs]

       -- create a timer that updates the ball positions
       t <- timer f [interval := 20, on command := nextBalls vballs p]

       -- react on user input
       set p [on click         := dropBall vballs p              -- drop ball
             ,on clickRight    := (\pt -> ballsFrame)            -- new window
             ,on (charKey 'p') := set t [enabled   :~ not]        -- pause
             ,on (charKey '-') := set t [interval :~ \i -> i*2]  -- increase interval
             ,on (charKey '+') := set t [interval :~ \i -> max 1 (i `div` 2)]
             ]

       -- put the panel in the frame, with a minimal size
       set f [layout := minsize (sz maxX maxY) $ widget p]
   where
   	-- paint the balls
    paintBalls :: Var [[Point]] -> DC a -> Rect -> IO ()
    paintBalls vballs dc viewArea
      = do balls <- varGet vballs
           set dc [brushColor := red, brushKind := BrushSolid]
           mapM_ (drawBall dc) [p | (p:ps) <- balls]

    drawBall dc pt
      = circle dc pt radius []
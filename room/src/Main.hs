import Graphics.UI.WX hiding (Object)
import Prelude hiding ((*>))

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

main :: IO ()
main = start gui

gui :: IO ()
gui = do
  wnd <- frame [ text := "Room" ] 
  return ()
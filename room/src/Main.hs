import Graphics.UI.WX
import Data.Bits

main :: IO ()
main = start gui

gui :: IO ()
gui = do
  wnd <- frame [ text := "Room" ] 
  return ()
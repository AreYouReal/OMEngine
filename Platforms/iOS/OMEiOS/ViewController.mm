#import "ViewController.h"
#include "SRUtils.h"
#include "Game.h"


@interface ViewController (){
    SRContext _SRContext;
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

- (void)viewDidLoad{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

    if (!self.context){
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)dealloc{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    // Dispose of any resources that can be recreated.
}

- (void)setupGL{
    [EAGLContext setCurrentContext:self.context];
    
    memset( &_context, 0, sizeof( _context ) );
  
    Game::Main(&_SRContext);
}

- (void)tearDownGL{
    [EAGLContext setCurrentContext:self.context];

    if ( _SRContext.shutdownFunc ){
        _SRContext.shutdownFunc( &_SRContext );
    }
}


- (void)update{
    if ( _SRContext.updateFunc ){
        _SRContext.updateFunc( &_SRContext, self.timeSinceLastUpdate );
    }
}


// Question here: Why use rect? Why rect.size = view.size / 3 ???
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
    _SRContext.width = rect.size.width;
    _SRContext.height = rect.size.height;
    
    if ( _SRContext.drawFunc ){
        _SRContext.drawFunc( &_SRContext );
    }
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    //    NSLog(@"touchBEGAN, %f, %f", location.x, location.y);
    if(_SRContext.touchFunc) _SRContext.touchFunc(&_SRContext, (int)TouchState::BEGIN, location.x, location.y);
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    
    if(_SRContext.touchFunc) _SRContext.touchFunc(&_SRContext, (int)TouchState::MOVED, location.x, location.y);
//        NSLog(@"touchMOVED, %f, %f", location.x, location.y);
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    //    NSLog(@"touchCANCELLED, %f, %f", location.x, location.y);
    if(_SRContext.touchFunc) _SRContext.touchFunc(&_SRContext, (int)TouchState::CANCELLED, location.x, location.y);
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    //    NSLog(@"touchENDED, %f, %f", location.x, location.y);
    if(_SRContext.touchFunc)  _SRContext.touchFunc(&_SRContext, (int)TouchState::ENDED, location.x, location.y);
}

@end

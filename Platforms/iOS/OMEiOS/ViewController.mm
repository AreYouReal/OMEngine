#import "ViewController.h"
#include "OMUtils.h"
#include "OMGame.h"


@interface ViewController (){
    OMContext _OMContext;
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
    _OMContext.width = [[self view] bounds].size.width;
    _OMContext.height = [[self view] bounds].size.height;
    
    OMGame::StartUp(&_OMContext);
}

- (void)tearDownGL{
    [EAGLContext setCurrentContext:self.context];

    if ( _OMContext.shutdownFunc ){
        _OMContext.shutdownFunc();
    }
}


- (void)update{
    if ( _OMContext.updateFunc ){
        _OMContext.updateFunc( self.timeSinceLastUpdate );
    }
}


// Question here: Why use rect? Why rect.size = view.size / 3 ???
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
    _OMContext.width = view.drawableWidth;
    _OMContext.height = view.drawableHeight;
    
    if ( _OMContext.drawFunc ){
        _OMContext.drawFunc( );
    }
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    //    NSLog(@"touchBEGAN, %f, %f", location.x, location.y);
    if(_OMContext.touchFunc) _OMContext.touchFunc((int)TouchState::BEGIN, location.x, location.y);
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    
    if(_OMContext.touchFunc) _OMContext.touchFunc((int)TouchState::MOVED, location.x, location.y);
//        NSLog(@"touchMOVED, %f, %f", location.x, location.y);
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    //    NSLog(@"touchCANCELLED, %f, %f", location.x, location.y);
    if(_OMContext.touchFunc) _OMContext.touchFunc((int)TouchState::CANCELLED, location.x, location.y);
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    //    NSLog(@"touchENDED, %f, %f", location.x, location.y);
    if(_OMContext.touchFunc)  _OMContext.touchFunc((int)TouchState::ENDED, location.x, location.y);
}

@end

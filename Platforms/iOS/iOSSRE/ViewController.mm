#import "ViewController.h"
#include "SRUtils.h"
#include "Main.h"


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
  
    Main(&_SRContext);
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

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
    _SRContext.width = view.drawableWidth;
    _SRContext.height = view.drawableHeight;
    
    if ( _SRContext.drawFunc ){
        _SRContext.drawFunc( &_SRContext );
    }
}


@end

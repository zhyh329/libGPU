/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_visionin_gpu_GPU */
#include "bs.h"
#include "GPU.h"
#include "GPUStreamFrame.h"

#ifdef __cplusplus
extern "C" {
#endif
void initialize(JNIEnv* env, jclass jc, jobject context, jstring jlicense);

void resource(JNIEnv* env, jclass jc, jobject asset);
jint createTexture(JNIEnv* env, jclass jc);
void destroyTexture(JNIEnv* env, jclass jc, jint ji);

jlong eglContext(JNIEnv * env, jobject jo, jboolean active);
void setRenderSurface(JNIEnv * env, jobject jo, jobject jobj);
void makeCurrent(JNIEnv * env, jobject jo, jlong jl);
void destroyEGL(JNIEnv * env, jobject jo, jlong jl);

void processTexture(JNIEnv * env, jobject jo, jint texture, jint texture_type);
void processPicture(JNIEnv * env, jobject jo, jstring jpath);
void processBytes(JNIEnv * env, jobject jo, jbyteArray jbytes, jint width, jint height, jint format);
void getBytes(JNIEnv* env, jobject jo, jbyteArray jb);
int getTexture(JNIEnv* env, jobject jo);

void setOutputSize(JNIEnv * env, jobject jo, jint width, jint height);
void setOutputFormat(JNIEnv * env, jobject jo, jint format);

void setInputSize(JNIEnv * env, jobject jo, jint width, jint height);
void setInputRotation(JNIEnv * env, jobject jo, jint rotation);

void setViewRotation(JNIEnv * env, jobject jo, jint rotation);
void setPreviewMirror(JNIEnv * env, jobject jo, jboolean mirror);
void setOutputMirror(JNIEnv * env, jobject jo, jboolean mirror);
void setOutputRotation(JNIEnv * env, jobject jo, jint rotation);
void setFrameRotation(JNIEnv* env, jobject jo, jint rotation);

void setSmoothStrength(JNIEnv * env, jobject jo, jfloat level);
void setWhitenStrength(JNIEnv * env, jobject jo, jfloat level);

void setExtraFilter(JNIEnv * env, jobject jo, jstring jname);
void closeExtraFilter(JNIEnv * env, jobject jo);
void setExtraParameter(JNIEnv * env, jobject jo, jfloat para);

void setColorFilter(JNIEnv * env, jobject jo, jint f, jfloat strength);
void setUnBlurRegion(JNIEnv * env, jobject jo, jint x, jint y, jint radius);

void setOutputView(JNIEnv * env, jobject jo);
void removeOutputView(JNIEnv * env, jobject jo);
void setFrameSize(JNIEnv* env, jobject jo, jint width, jint height);
void setViewFillMode(JNIEnv* env, jobject jo, jint mode);

// blend用于logo
void setPreviewBlend(JNIEnv * env, jobject jo, jstring jpath, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror);
void setPreviewBlendBitmap(JNIEnv * env, jobject jo, jobject bitmap, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror);
void setVideoBlend(JNIEnv * env, jobject jo, jstring jpath, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror);
void setVideoBlendBitmap(JNIEnv * env, jobject jo, jobject bitmap, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror);

void setBlank(JNIEnv* env, jobject jo, jint border, jint r, jint g, jint b);
void setPreviewColor(JNIEnv* env, jobject jo, jint r, jint g, jint b);
#ifdef __cplusplus
}
#endif

static GPUTextureInput*  g_texture_input = NULL;
static GPURawInput*      g_raw_input = NULL;
static GPUView*          g_view = NULL;

// 创建Android NV21 Texture
jint createTexture(JNIEnv * env, jclass jc){
	GLuint texture;
	GPUContext::shareInstance()->makeCurrent();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	//glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;
}

// 释放texture
void destroyTexture(JNIEnv * env, jclass jc, jint texture){
	GPUContext::shareInstance()->makeCurrent();
	glDeleteTextures(1, (GLuint*)&texture);
}

// 创建glContext
jlong eglContext(JNIEnv * env, jobject jo, jboolean active){
    bs_log_init("stdout");
	GPUContext::setContextEnable(active);
	GPUContext* context  = GPUContext::shareInstance();
	context->makeCurrent();

	GPUStreamFrame* stream = new GPUStreamFrame();
	return (jlong)context;
}

void setRenderSurface(JNIEnv * env, jobject jo, jobject jobj){
	ANativeWindow* window = ANativeWindow_fromSurface(env, jobj);
	if (window==NULL)
	{
		err_log("SurfaceWindow is NULL!");
		return;
	}
	if(g_view==NULL){
		g_view = new GPUView(window);
		GPUStreamFrame::shareInstance()->setOutputView(g_view);
	}
	else{
		g_view->setNativeWindow(window);
	}
}

void makeCurrent(JNIEnv * env, jobject jo, jlong jcontext){
	GPUContext::shareInstance()->makeCurrent();
}

void destroyEGL(JNIEnv * env, jobject jo, jlong jcontext){
	GPUContext::shareInstance()->makeCurrent();

	if (g_texture_input!=NULL)
	{
		delete g_texture_input;
		g_texture_input = NULL;
	}

	if (g_raw_input!=NULL)
	{
		delete g_raw_input;
		g_raw_input = NULL;
	}

	if (g_view!=NULL)
	{
		delete g_view;
		g_view = NULL;
	}

	GPUStreamFrame::destroyInstance();
	GPUBufferCache::destroyInstance();
	info_log("Destroy Buffer Cache Success!");
	GPUVertexBufferCache::destroyInstance();
	info_log("Destroy Vertex Cache Success!");
	// 销毁，注意这里要将静态变量改为null
	GPUContext::destroyInstance();
	info_log("Destroy Surface Success!");
}

void processTexture(JNIEnv * env, jobject jo, jint texture, jint texture_type){
	if (g_texture_input==NULL)
	{
		GPUStreamFrame* stream = GPUStreamFrame::shareInstance();
		if (stream->m_frame_width==0 || stream->m_frame_height==0)
		{
			err_log("Visionin Error: don't set videosize!");
			return;
		}
		g_texture_input = new GPUTextureInput(stream->m_frame_width, stream->m_frame_height, texture_type);
		g_texture_input->setOutputRotation(stream->getOutputRotation());
		//stream->setInputFilter(g_texture_input);
		g_texture_input->addTarget(stream);
	}

    GPUContext::shareInstance()->runAsyncTasks();
	g_texture_input->processTexture(texture);
}

void processPicture(JNIEnv * env, jobject jo, jstring jpath){
	const char* path = env->GetStringUTFChars(jpath, NULL);
	if(path == NULL || strlen(path)==0) {
		return;
	}

	GPUStreamFrame* stream = GPUStreamFrame::shareInstance();

	GPUPicture pic(path);
	pic.addTarget(stream);

	stream->m_input->setOutputRotation(stream->getOutputRotation());
	pic.processImage();
    glFinish();
	pic.removeAllTargets();
	env->ReleaseStringUTFChars(jpath, path);
}

void processBytes(JNIEnv * env, jobject jo, jbyteArray jbytes, jint width, jint height, jint format){
	jbyte* bytes = (jbyte*)env->GetPrimitiveArrayCritical(jbytes, NULL);
	if (bytes==NULL)
	{
		err_log("bytes is NULL!");
		return;
	}

	if (g_raw_input==NULL)
	{
		GPUStreamFrame* stream = GPUStreamFrame::shareInstance();
		g_raw_input = new GPURawInput();
        g_raw_input->setOutputRotation(stream->getOutputRotation());
		//stream->setInputFormat((GPUPixelFormat_t)format);
		g_raw_input->addTarget(stream);
	}

	g_raw_input->uploadBytes((GLubyte*)bytes, width, height, (gpu_pixel_format_t)format);
	env->ReleasePrimitiveArrayCritical(jbytes, bytes, 0);
}

void getBytes(JNIEnv * env, jobject jobj, jbyteArray jbytes){
	jbyte* bytes = (jbyte*)env->GetPrimitiveArrayCritical(jbytes, NULL);
	if (bytes==NULL)
	{
		err_log("bytes is NULL!");
		return;
	}

	GPUStreamFrame::shareInstance()->m_raw_output->getBuffer((unsigned char*)bytes, env->GetArrayLength(jbytes));
	env->ReleasePrimitiveArrayCritical(jbytes, bytes, 0);
}

int getTexture(JNIEnv* env, jobject jo){
	return GPUStreamFrame::shareInstance()->m_raw_output->getTexture();
}

void setOutputSize(JNIEnv * env, jobject jo, jint width, jint height){
	GPUStreamFrame::shareInstance()->setOutputSize(width, height);
}

void setOutputFormat(JNIEnv * env, jobject jo, jint format){
	GPUStreamFrame::shareInstance()->setOutputFormat((gpu_pixel_format_t)format);
}

void setInputSize(JNIEnv * env, jobject jo, jint width, jint height){
	if (g_texture_input!=NULL)
	{
		g_texture_input->setOutputSize((uint32_t)width, (uint32_t)height);
	}
	GPUStreamFrame::shareInstance()->setInputSize((uint32_t)width, (uint32_t)height);
}
void setInputRotation(JNIEnv * env, jobject jo, jint rotation){
	GPUStreamFrame::shareInstance()->setInputRotation((gpu_rotation_t)rotation);
	if (g_texture_input!=NULL){
		g_texture_input->setOutputRotation((gpu_rotation_t)rotation);
	}
}

void setPreviewMirror(JNIEnv * env, jobject jo, jboolean mirror){
	GPUStreamFrame::shareInstance()->setPreviewMirror(mirror);
}
void setOutputMirror(JNIEnv * env, jobject jo, jboolean mirror){
	GPUStreamFrame::shareInstance()->setOutputMirror(mirror);
}
void setOutputRotation(JNIEnv * env, jobject jo, jint rotation){
	GPUStreamFrame::shareInstance()->setOutputRotation((gpu_rotation_t)rotation);
}
void setFrameRotation(JNIEnv* env, jobject jo, jint rotation){
    GPUStreamFrame::shareInstance()->setFrameRotation((gpu_rotation_t)rotation);
}
void setSmoothStrength(JNIEnv * env, jobject jo, jfloat level){
	GPUStreamFrame::shareInstance()->setSmoothStrength(level);
}
void setWhitenStrength(JNIEnv * env, jobject jo, jfloat level){
    GPUStreamFrame::shareInstance()->setWhitenStrength(level);
}

void setOutputView(JNIEnv *env, jobject obj){
	GPUStreamFrame::shareInstance()->setOutputView(g_view);
}

void removeOutputView(JNIEnv * env, jobject obj){
	GPUStreamFrame::shareInstance()->removeOutputView();
}

void setExtraFilter(JNIEnv * env, jobject obj, jstring filter){
	const char* name = env->GetStringUTFChars(filter, NULL);
    if(name == NULL || strlen(name)==0) {
        return;
    }

    GPUStreamFrame::shareInstance()->setExtraFilter(name);

	env->ReleaseStringUTFChars(filter, name);
}

void setColorFilter(JNIEnv * env, jobject jo, jint filter, jfloat strength){
	switch(filter){
		case GPU_COLOR_CONTRAST_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setContrast(strength);
			break;
		case GPU_COLOR_GAMMA_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setGamma(strength);
			break;
		case GPU_COLOR_SATURATION_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setSaturation(strength);
			break;
		case GPU_COLOR_FADE_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setFade(strength);
			break;
		case GPU_COLOR_BLUR_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setBlur(strength);
			break;
		case GPU_COLOR_SHARPNESS_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setSharpness(strength);
			break;
		case GPU_COLOR_TEMPERATURE_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setTemperature(strength);
			break;
		case GPU_COLOR_TINT_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setTint(strength);
			break;
		case GPU_COLOR_HIGHLIGHTS_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setHighlights(strength);
			break;
		case GPU_COLOR_SHADOWS_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setShadows(strength);
			break;
		case GPU_COLOR_VIGNETTE_FILTER:
			GPUStreamFrame::shareInstance()->m_color_filter.setVignette(strength);
			break;
        default:
            err_log("unkown color filter: %d", filter);
	}
}

void setUnBlurRegion(JNIEnv * env, jobject jo, jint x, jint y, jint radius){
	GPUStreamFrame::shareInstance()->m_color_filter.setUnBlurRegion(x, y, radius);
}

void setViewRotation(JNIEnv * env, jobject jo, jint rotation){
	if (GPUStreamFrame::shareInstance()->m_view!=NULL)
	{
		GPUStreamFrame::shareInstance()->m_view->setOutputRotation((gpu_rotation_t)rotation);
	}
	else{
		err_log("Visionin View: view has not been new.");
	}
}

void setFrameSize(JNIEnv* env, jobject jo, jint width, jint height){
	//GPUStreamFrame::shareInstance()->m_blank_filter.setFillMode(GPUFillModePreserveAspectRatioAndFill);
	GPUStreamFrame::shareInstance()->setStreamFrameSize(width, height);
}

void setViewFillMode(JNIEnv* env, jobject jo, jint mode){
    if(GPUStreamFrame::shareInstance()->m_view!=NULL){
        ((GPUView*)GPUStreamFrame::shareInstance()->m_view)->setFillMode((gpu_fill_mode_t)mode);
    }
}

void closeExtraFilter(JNIEnv *, jobject){
	GPUStreamFrame::shareInstance()->removeExtraFilter();
}
void setExtraParameter(JNIEnv * env, jobject obj, jfloat para){
	GPUStreamFrame::shareInstance()->setExtraParameter(para);
}

void setPreviewBlend(JNIEnv * env, jobject jo, jstring jpath, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror){
	GPUStreamFrame* stream = GPUStreamFrame::shareInstance();
	gpu_rect_t rect;
	if (jpath==NULL){
		stream->setPreviewBlend(NULL, rect, mirror);
		return;
	}

	const char* path = env->GetStringUTFChars(jpath, NULL);
	rect.pointer.x = x;
	rect.pointer.y = y;
	rect.size.width = w;
	rect.size.height = h;
	GPUPicture* pic = new GPUPicture(path);
	stream->setPreviewBlend(pic, rect, mirror);

	env->ReleaseStringUTFChars(jpath, path);
}
void setPreviewBlendBitmap(JNIEnv * env, jobject jo, jobject bitmap, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror){
    GPUStreamFrame* stream = GPUStreamFrame::shareInstance();
    gpu_rect_t rect;

    rect.pointer.x = x;
    rect.pointer.y = y;
    rect.size.width = w;
    rect.size.height = h;
    GPUPicture* pic = new GPUPicture(bitmap);
    stream->setPreviewBlend(pic, rect, mirror);
}

void setVideoBlend(JNIEnv * env, jobject jo, jstring jpath, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror){
	GPUStreamFrame* stream = GPUStreamFrame::shareInstance();
	gpu_rect_t rect;
	if (jpath==NULL){
		stream->setVideoBlend(NULL, rect, mirror);
		return;
	}

	const char* path = env->GetStringUTFChars(jpath, NULL);
	rect.pointer.x = x;
	rect.pointer.y = y;
	rect.size.width = w;
	rect.size.height = h;
	GPUPicture* pic = new GPUPicture(path);
	err_log("path: %s  pic: %llu", path, (unsigned long long)pic);
	stream->setVideoBlend(pic, rect, mirror);

	env->ReleaseStringUTFChars(jpath, path);
}

void setVideoBlendBitmap(JNIEnv * env, jobject jo, jobject bitmap, jfloat x, jfloat y, jfloat w, jfloat h, jboolean mirror){
    GPUStreamFrame* stream = GPUStreamFrame::shareInstance();
    gpu_rect_t rect;

    rect.pointer.x = x;
    rect.pointer.y = y;
    rect.size.width = w;
    rect.size.height = h;
    GPUPicture* pic = new GPUPicture(bitmap);
    stream->setVideoBlend(pic, rect, mirror);
}

void setBlank(JNIEnv* env, jobject jo, jint border, jint r, jint g, jint b){
	GPUStreamFrame::shareInstance()->setBlank(border, r, g, b);
}

void setPreviewColor(JNIEnv* env, jobject jo, jint r, jint g, jint b){
    if(g_view != NULL){
        g_view->setClearColor(r/255.0, g/255.0, b/255.0);
    }
}
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
	return JNI_VERSION_1_4;
}
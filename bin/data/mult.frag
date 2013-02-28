uniform sampler2DRect tex1; //テクスチャの変数名をuniform型で宣言
uniform sampler2DRect tex2; //テクスチャの変数名をuniform型で宣言

void main( void )
{  
	  vec2 xy = gl_TexCoord[0].xy;//テクスチャ座標を取得 0 ~ 1.0
      vec4 color1 = texture2DRect(tex1,xy);
      vec4 color2 = texture2DRect(tex2,xy);
      
      gl_FragColor = color1 * color2;
}
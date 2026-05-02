package com.fireaim;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.content.Context;
import android.opengl.GLSurfaceView;

public class FireaimRenderer implements GLSurfaceView.Renderer {
    
    private float playerYaw = 0;
    private float playerPitch = 0;
    private float playerX = 0, playerY = 1.7f, playerZ = 0;
    
    private List<Target3D> targets = new ArrayList<>();
    private List<Projectile3D> projectiles = new ArrayList<>();
    private Random random = new Random();
    
    private int score = 0;
    private int wave = 1;
    private int combo = 0;
    private float lastHitTime = 0;
    private boolean gameOver = false;
    
    private float[] groundColor = {0.1f, 0.1f, 0.18f, 1.0f};
    private float[][] pillarColors = {
        {0.08f, 0.08f, 0.15f},
        {0.1f, 0.12f, 0.2f},
        {0.06f, 0.08f, 0.15f}
    };
    
    private float[][] targetColors = {
        {1.0f, 0.0f, 0.5f},
        {0.0f, 1.0f, 1.0f},
        {1.0f, 0.67f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f}
    };
    
    private float projectileColor[] = {0.0f, 1.0f, 0.53f, 1.0f};
    
    public FireaimRenderer() {
        spawnWave();
    }
    
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        gl.glDisable(GL10.GL_DITHER);
        gl.glEnable(GL10.GL_DEPTH_TEST);
        gl.glEnable(GL10.GL_BLEND);
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);
        
        float[] ambient = {0.2f, 0.2f, 0.3f, 1.0f};
        gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_AMBIENT, ambient, 0);
        
        float[] lightDir = {0.5f, 1.0f, 0.5f, 0.0f};
        gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_DIRECTION, lightDir, 0);
        gl.glEnable(GL10.GL_LIGHT0);
    }
    
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0, 0, width, height);
        gl.glMatrixMode(GL10.GL_PROJECTION);
        gl.glLoadIdentity();
        GLU.gluPerspective(gl, 75, (float)width/height, 0.1f, 1000);
    }
    
    public void onDrawFrame(GL10 gl) {
        gl.glClearColor(0.04f, 0.04f, 0.1f, 1.0f);
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        
        gl.glMatrixMode(GL10.GL_MODELVIEW);
        gl.glLoadIdentity();
        
        setupCamera(gl);
        renderGround(gl);
        renderPillars(gl);
        
        float currentTime = System.currentTimeMillis() / 1000.0f;
        
        for (Target3D target : targets) {
            target.update(currentTime);
            renderTarget(gl, target);
        }
        
        for (Projectile3D proj : projectiles) {
            proj.update();
            renderProjectile(gl, proj);
        }
        
        projectiles.removeIf(p -> p.isExpired());
        
        checkCollisions();
        
        if (targets.isEmpty() && !gameOver) {
            nextWave();
        }
    }
    
    private void setupCamera(GL10 gl) {
        float lookX = (float)(Math.sin(playerYaw) * Math.cos(playerPitch));
        float lookY = (float)Math.sin(playerPitch);
        float lookZ = (float)(Math.cos(playerYaw) * Math.cos(playerPitch));
        
        GLU.gluLookAt(gl, playerX, playerY, playerZ, 
                      playerX + lookX, playerY + lookY, playerZ + lookZ,
                      0, 1, 0);
    }
    
    private void renderGround(GL10 gl) {
        gl.glColor4f(groundColor[0], groundColor[1], groundColor[2], groundColor[3]);
        
        float[] vertices = {
            -50, -0.25f, -50,
             50, -0.25f, -50,
             50, -0.25f,  50,
            -50, -0.25f,  50
        };
        
        FloatBuffer buffer = makeFloatBuffer(vertices);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, buffer);
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
        
        short[] indices = {0, 1, 2, 0, 2, 3};
        ByteBuffer ib = ByteBuffer.allocateShort(indices.length * 2);
        ib.order(ByteOrder.nativeOrder());
        ib.asShortBuffer().put(indices);
        ib.position(0);
        
        gl.glDrawElements(GL10.GL_TRIANGLES, 6, GL10.GL_UNSIGNED_SHORT, ib);
    }
    
    private void renderPillars(GL10 gl) {
        for (int i = 0; i < 50; i++) {
            float x = ((i * 7 % 40) - 20) + (i % 3) * 5;
            float z = ((i * 13 % 40) - 20) + (i / 3 % 4) * 5;
            float height = 2 + (i % 8) * 1.5f;
            
            int colorIdx = i % 3;
            gl.glColor4f(pillarColors[colorIdx][0], pillarColors[colorIdx][1], pillarColors[colorIdx][2], 1.0f);
            
            renderBox(gl, x - 0.5f, 0, z - 0.5f, 1, height, 1);
        }
    }
    
    private void renderBox(GL10 gl, float x, float y, float z, float w, float h, float d) {
        float[] vertices = {
            x, y, z,      x+w, y, z,    x+w, y+h, z,  x, y+h, z,
            x, y, z+d,    x+w, y, z+d,  x+w, y+h, z+d, x, y+h, z+d
        };
        
        FloatBuffer buffer = makeFloatBuffer(vertices);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, buffer);
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
        
        short[] indices = {
            0,1,2, 0,2,3, 4,5,6, 4,6,7,
            0,1,5, 0,5,4, 2,3,7, 2,7,6,
            0,3,7, 0,7,4, 1,5,6, 1,6,2
        };
        
        ByteBuffer ib = ByteBuffer.allocateShort(indices.length * 2);
        ib.order(ByteOrder.nativeOrder());
        ib.asShortBuffer().put(indices);
        ib.position(0);
        
        gl.glDrawElements(GL10.GL_TRIANGLES, 36, GL10.GL_UNSIGNED_SHORT, ib);
    }
    
    private void renderTarget(GL10 gl, Target3D target) {
        int colorIdx = Math.abs(target.id.hashCode()) % 5;
        gl.glColor4f(targetColors[colorIdx][0], targetColors[colorIdx][1], targetColors[colorIdx][2], 1.0f);
        
        renderSphere(gl, target.x, target.y, target.z, 0.5f);
        
        gl.glColor4f(targetColors[colorIdx][0], targetColors[colorIdx][1], targetColors[colorIdx][2], 0.3f);
        renderSphere(gl, target.x, target.y, target.z, 0.6f);
    }
    
    private void renderSphere(GL10 gl, float x, float y, float z, float radius) {
        int stacks = 10;
        int slices = 10;
        
        for (int i = 0; i < stacks; i++) {
            float lat1 = (float)Math.PI * i / stacks;
            float lat2 = (float)Math.PI * (i + 1) / stacks;
            
            for (int j = 0; j < slices; j++) {
                float lon1 = (float)(2 * Math.PI * j / slices);
                float lon2 = (float)(2 * Math.PI * (j + 1) / slices);
                
                float[] vertices = {
                    x + radius * (float)(Math.sin(lat1) * Math.cos(lon1)),
                    y + radius * (float)Math.cos(lat1),
                    z + radius * (float)(Math.sin(lat1) * Math.sin(lon1)),
                    
                    x + radius * (float)(Math.sin(lat1) * Math.cos(lon2)),
                    y + radius * (float)Math.cos(lat1),
                    z + radius * (float)(Math.sin(lat1) * Math.sin(lon2)),
                    
                    x + radius * (float)(Math.sin(lat2) * Math.cos(lon2)),
                    y + radius * (float)Math.cos(lat2),
                    z + radius * (float)(Math.sin(lat2) * Math.sin(lon2)),
                    
                    x + radius * (float)(Math.sin(lat2) * Math.cos(lon1)),
                    y + radius * (float)Math.cos(lat2),
                    z + radius * (float)(Math.sin(lat2) * Math.sin(lon1))
                };
                
                FloatBuffer buffer = makeFloatBuffer(vertices);
                gl.glVertexPointer(3, GL10.GL_FLOAT, 0, buffer);
                gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
                
                short[] indices = {0, 1, 2, 0, 2, 3};
                ByteBuffer ib = ByteBuffer.allocateShort(indices.length * 2);
                ib.order(ByteOrder.nativeOrder());
                ib.asShortBuffer().put(indices);
                ib.position(0);
                
                gl.glDrawElements(GL10.GL_TRIANGLES, 6, GL10.GL_UNSIGNED_SHORT, ib);
            }
        }
    }
    
    private void renderProjectile(GL10 gl, Projectile3D proj) {
        gl.glColor4f(projectileColor[0], projectileColor[1], projectileColor[2], projectileColor[3]);
        
        gl.glPointSize(8);
        float[] point = {proj.x, proj.y, proj.z};
        FloatBuffer pBuffer = makeFloatBuffer(point);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, pBuffer);
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
        gl.glDrawArrays(GL10.GL_POINTS, 0, 1);
    }
    
    private FloatBuffer makeFloatBuffer(float[] arr) {
        ByteBuffer bb = ByteBuffer.allocateDirect(arr.length * 4);
        bb.order(ByteOrder.nativeOrder());
        FloatBuffer fb = bb.asFloatBuffer();
        fb.put(arr);
        fb.position(0);
        return fb;
    }
    
    private void spawnWave() {
        int count = 10 + (wave - 1) * 2;
        for (int i = 0; i < count; i++) {
            spawnTarget();
        }
    }
    
    private void spawnTarget() {
        double angle = random.nextDouble() * 2 * Math.PI;
        double distance = 5 + random.nextDouble() * 15;
        float x = (float)(Math.cos(angle) * distance);
        float z = (float)(Math.sin(angle) * distance);
        float y = 1 + random.nextFloat() * 3;
        float speed = 2 + (wave - 1) * 0.5f;
        
        targets.add(new Target3D("target_" + random.nextInt(), x, y, z, speed));
    }
    
    public void updateLook(float dyaw, float dpitch) {
        playerYaw += dyaw;
        playerPitch = Math.max(-Math.PI/3, Math.min(Math.PI/3, playerPitch + dpitch));
    }
    
    public void shoot() {
        float currentTime = System.currentTimeMillis() / 1000.0f;
        
        float dirX = (float)(Math.sin(playerYaw) * Math.cos(playerPitch));
        float dirY = (float)Math.sin(playerPitch);
        float dirZ = (float)(Math.cos(playerYaw) * Math.cos(playerPitch));
        
        projectiles.add(new Projectile3D(playerX, playerY, playerZ, dirX, dirY, dirZ, currentTime));
    }
    
    private void checkCollisions() {
        float currentTime = System.currentTimeMillis() / 1000.0f;
        
        for (Projectile3D proj : projectiles) {
            for (Target3D target : targets) {
                float dx = proj.x - target.x;
                float dy = proj.y - target.y;
                float dz = proj.z - target.z;
                float dist = (float)Math.sqrt(dx*dx + dy*dy + dz*dz);
                
                if (dist < 0.5f) {
                    target.active = false;
                    proj.expired = true;
                    
                    int comboMult = 1;
                    if (currentTime - lastHitTime < 2.0f) {
                        comboMult = Math.min(combo + 1, 5);
                    }
                    
                    score += 100 * comboMult;
                    combo = comboMult;
                    lastHitTime = currentTime;
                    
                    targets.remove(target);
                    break;
                }
            }
        }
        
        projectiles.removeIf(p -> p.expired);
        targets.removeIf(t -> !t.active);
    }
    
    private void nextWave() {
        if (wave >= 5) {
            gameOver = true;
        } else {
            wave++;
            spawnWave();
        }
    }
    
    public void resetGame() {
        score = 0;
        wave = 1;
        combo = 0;
        gameOver = false;
        targets.clear();
        projectiles.clear();
        playerYaw = 0;
        playerPitch = 0;
        spawnWave();
    }
    
    public int getScore() { return score; }
    public int getWave() { return wave; }
    public int getCombo() { return combo; }
    public boolean isGameOver() { return gameOver; }
    
    static class Target3D {
        String id;
        float x, y, z, speed;
        float startX, startZ;
        float angle = 0;
        boolean active = true;
        
        Target3D(String id, float x, float y, float z, float speed) {
            this.id = id;
            this.x = x;
            this.y = y;
            this.z = z;
            this.speed = speed;
            this.startX = x;
            this.startZ = z;
        }
        
        void update(float time) {
            angle = time * speed * 0.5f;
            x = (float)(startX * Math.cos(angle) - startZ * Math.sin(angle));
            z = (float)(startX * Math.sin(angle) + startZ * Math.cos(angle));
        }
    }
    
    static class Projectile3D {
        float x, y, z;
        float dirX, dirY, dirZ;
        float createdAt;
        boolean expired = false;
        
        Projectile3D(float x, float y, float z, float dx, float dy, float dz, float time) {
            this.x = x;
            this.y = y;
            this.z = z;
            this.dirX = dx;
            this.dirY = dy;
            this.dirZ = dz;
            this.createdAt = time;
        }
        
        void update() {
            x += dirX * 50 * 0.016f;
            y += dirY * 50 * 0.016f;
            z += dirZ * 50 * 0.016f;
        }
        
        boolean isExpired() {
            return expired || (System.currentTimeMillis() / 1000.0f - createdAt > 3.0f);
        }
    }
}
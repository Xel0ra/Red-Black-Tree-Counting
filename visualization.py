import matplotlib.pyplot as plt
import numpy as np

# 性能测试数据
# 注意：BF 算法由于存在 ERROR 且数据点太少，不适合在 Log-Log 图中比较，已排除。
# DP 在 N=100000 时标记为 300s (T/L)
data = {
    'N': np.array([5, 10, 20, 50, 100, 1000, 5000, 10000, 50000, 100000]),
    'DP ($O(N^2 \log N)$)': np.array([0.012, 0.013, 0.011, 0.013, 0.013, 0.053, 0.734, 2.795, 78.109, 300.0]),
    'GF (递归乘法)': np.array([0.014, 0.013, 0.013, 0.014, 0.014, 0.052, 0.777, 2.373, 23.643, 76.087]),
    'NTT ($O(N \log^2 N)$)': np.array([0.015, 0.014, 0.014, 0.013, 0.016, 0.095, 0.706, 1.682, 7.650, 17.095])
}

# 用于标记超时的点
TIMEOUT_N = 100000

plt.figure(figsize=(10, 6))

# 绘制数据
for label, times in data.items():
    if label == 'N':
        continue
    
    # 绘制所有点和线
    plt.loglog(data['N'], times, 
               label=label, 
               marker='o', 
               markersize=6,
               linestyle='-',
               alpha=0.8)

# 突出显示 DP 的超时点
dp_timeout_index = np.where(data['N'] == TIMEOUT_N)[0][0]
plt.scatter(data['N'][dp_timeout_index], data['DP ($O(N^2 \log N)$)'][dp_timeout_index], 
            color='red', 
            marker='X', 
            s=100, 
            zorder=5, 
            label='DP 算法超时 ($\ge 300s$)')


# 设置图表属性
plt.title('红黑树计数算法性能对比 (Log-Log Scale)', fontsize=16)
plt.xlabel('内部节点数量 $N$ (对数坐标)', fontsize=14)
plt.ylabel('运行时间 (秒) (对数坐标)', fontsize=14)

# 调整刻度标签格式以更易读
x_ticks = [10, 100, 1000, 10000, 100000]
plt.xticks(x_ticks, [f'$10^{int(np.log10(x))}$' for x in x_ticks])
plt.yticks(fontsize=12)
plt.gca().tick_params(axis='both', which='major', labelsize=12)

# 添加网格
plt.grid(True, which="both", ls="--", linewidth=0.5)

plt.legend(loc='lower right', fontsize=12)
plt.tight_layout()

# 保存图像
plt.savefig('performance_comparison.png')

print("图表已生成: performance_comparison.png")
